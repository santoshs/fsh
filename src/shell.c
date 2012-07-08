#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <errno.h>
#include <syslog.h>
#include <libgen.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "shell.h"
#include "env.h"
#include "list.h"

char *progname;
unsigned int flags_global;
jobs_t jobs;
extern builtin_t builtin[];
int last_exit;

char *getprompt (void)
{
	return getenv("PS1");
}

void history (char *line)
{
	if (line)
		add_history(line);
}

/* The main signal handler for the process group */

void put_command (command_t *cmd)
{
	args_t *args;
	command_t *c;
	struct list_head *arg_pos, *cmd_pos, *arg_i, *cmd_i;

	if (!cmd)
		return;

	list_for_each_safe(cmd_pos, cmd_i, &cmd->list) {
		c = list_entry(cmd_pos, command_t, list);
		list_for_each_safe(arg_pos, arg_i, &c->arg_list.list) {
			args = list_entry(arg_pos, args_t, list);
			list_del(arg_pos);
			free(args->arg);
			free(args);
		}
		list_del(cmd_pos);
		free(c);
	}
	free(cmd);
}

void queue_job (command_t *cmd)
{
	jobs_t *j;
	int i = 0;

	j = malloc(sizeof(jobs_t));
	j->job = cmd;
	j->pid = (list_entry(cmd->list.prev, command_t, list)->pid);
	j->jid = list_count(&jobs.list);
	j->line = strdup(global_line);
	list_add_tail(&j->list, &jobs.list);
}

/*
 * TODO Redirections, pipes
 */
int prepare_exec_extern(command_t *cmd)
{
	return 0;
}

bool is_builtin(command_t *cmd)
{
	int i = 0;

	while (i < MAX_BUILTINS) {
		if (!strcmp(builtin[i].cmd_str, cmd->argv[0])) {
			return TRUE;
		}
		i++;
	}

	return FALSE;
}
int exec_builtin(command_t *cmd)
{
	int i = 0;

	while(i < MAX_BUILTINS) {
		if (!strcmp(builtin[i].cmd_str, cmd->argv[0])) {
			return builtin[i].builtin_cb(cmd);
		}
		i++;
	}
	return -1;
}

int exec_extern (command_t *cmd)
{
	pid_t pid;
	int status = 0;

	pid = fork();
	if (pid == 0) {
		debug(BASIC_DEBUG, "Executing command\n");
		if (execvp(cmd->argv[0], cmd->argv) < 0) {
			perror(progname);
		/* We don't want the child also to start reading input and
		 * fork children if exec fails*/
			exit(errno);
		}
	}
	/* No else part here, only the parent can reach here */
	cmd->pid = pid;
	if (! cmd->flags & BACKGROUND_JOB) {
		waitpid(pid, &status, 0);
		cmd->retval = WIFEXITED(status) ? WEXITSTATUS(status) : 255;
		debug(BASIC_DEBUG, "Child exited with status %d\n", status);
	} else {
		putc('\n', stderr);
	}
	return status;
}

char **make_array_from_list (args_t *args, int count)
{
	args_t *t;
	int i = 0;
	char **argv;

	argv = malloc(count * sizeof(char *) + 1);
	i = 0;
	list_for_each_entry(t, &args->list, list) {
		argv[i++] = t->arg;
	}
	argv[i] = NULL;
	return argv;
}

void do_command (command_t *cmd)
{
	args_t *args;
	char **argv;
	int i;
	pid_t pid;
	int status = 0;

	if (!cmd) {
		return;
	}

	debug(BASIC_DEBUG, "Number of arguments %d\n", cmd->arg_count);
	cmd->argv = make_array_from_list(&cmd->arg_list, cmd->arg_count);

	if (is_builtin(cmd))
		status = exec_builtin(cmd);
	else {
		prepare_exec_extern(cmd);
		status = exec_extern(cmd);
	}

	if (cmd->flags & BACKGROUND_JOB) {
		queue_job(cmd);
	} else {
		last_exit = WEXITSTATUS(status);
		put_command(cmd);
	}

	current = NULL;
}

int main (int argc, char *argv[])
{
	progname = basename(argv[0]);
	openlog(progname, LOG_CONS, LOG_USER);
	if (env_init() < 0)
		return -1;

	fflush(stdin);
	syslog(LOG_INFO, "Shell initilisation\n");

	/* Initialise list for the jobs list */
	INIT_LIST_HEAD(&jobs.list);

	shell_loop();
	fprintf(stderr, "\n");

	closelog();

	return 0;
}
