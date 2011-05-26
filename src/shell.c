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

char *progname;
unsigned int flags_global;

LIST_HEAD(jobs);
LIST_HEAD(builtins);

char *getprompt (void)
{
	return get_env("PS1");
}

void history (char *line)
{
	if (line)
		add_history(line);
}

void put_command (command_t *cmd)
{
	args_t *args;
	struct list_head *pos, *s;

	list_for_each_safe(pos, s, &cmd->args.list) {
		args = list_entry(pos, args_t, list);
		list_del(pos);
		free(args->arg);
		free(args);
	}
	free(cmd);
}

enum job_status process (command_t *cmd)
{
	/*
	 * The command and arguments cannot directly be used as it can
	 * contain '>', '>>' and '|'. Find the first non-argument in
	 * the argument list end the arg list there
	 */
	/* for (i = 0; i < cmd->arg_count; i++) { */
	/* 	if (strcmp(cmd->args[i], ">") == 0) */

	//printf("%s\n", cmd->args[0]);
	//printf("%d\n", cmd->arg_count);
	return JOB_DONE;
}

/*
 * Redirections, pipes
 */
int prepare_exec_extern(command_t *cmd)
{
	return 0;
}

int exec_extern (command_t *cmd)
{
	pid_t pid;
	int status;

	pid = fork();
	if (pid == 0) {
		if (execvp(argv[0], argv) < 0)
			perror(progname);
		/* We don't want the child also to start reading input and
		 * fork children */
		exit(errno);
	}
	/* No else part here, only the parent can reach here */
	waitpid(pid, &status, 0); /* TODO Check background job here */
	debug(BASIC_DEBUG, "Child exited with status %d\n", status);
	return 0;
}

char **make_array_from_list (args_t *args, int *count)
{
	args_t *t;
	int i = 0;
	char **argv;

	/* TODO increment count while processing each word, we can avoid this
	 * loop */
	list_for_each_entry(t, &args->list, list) {
		i++;
	}

	printf("num_args = %d\n", i);
	i++;			/* We need a final NULL in the array */
	argv = malloc(i * sizeof(char *));
	i = 0;
	list_for_each_entry(t, &args->list, list) {
		argv[i++] = t->arg;
	}
	argv[i] = NULL;
	*count = i;
	return argv;
}

void do_command (command_t *cmd)
{
	args_t *args;
	char **argv;
	int i;
	pid_t pid;

	if (!cmd) {
		return;
	}

	argv = make_array_from_list(&cmd->args, &i);
	cmd->arg_count = i;
	debug(BASIC_DEBUG, "Number of arguments %d\n", i);
	if (exec_builtin(cmd)) {
		prepare_exec_extern(cmd);
		exec_extern(command);
	}

	put_command(cmd);
}

int main (int argc, char *argv[])
{
	progname = basename(argv[0]);

	openlog(progname, LOG_CONS, LOG_USER);

	if (env_init() < 0)
		return -1;

	fflush(stdin);
	syslog(LOG_INFO, "Shell initilisation\n");
	shell_loop();

	closelog();

	return 0;
}
