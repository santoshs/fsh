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

char *getprompt (void)
{
	return getenv("PS1");
}

void history (char *line)
{
	if (line)
		add_history(line);
}

command_t * get_command (void)
{
	command_t *cmd;
	static char *line;
	char *tmp;
	int ret;

	if (line) {
		free(line);
		line = NULL;
	}
	if ((line = readline(getprompt())) == NULL) {
		goto err1;
	}

	cmd = malloc(sizeof(command_t));
	if (!cmd) {
		free(line);
		line = NULL;
		goto err1;
	}
	memset(cmd, 0, sizeof(command_t));

	ret = parse_line(cmd, line);
	if (ret < 0) {		/* Empty line */
		return cmd;
	} else if (ret > 0) {
		debug(BASIC_DEBUG, "Parse error");
		/*
		 * Not going to exit shell for parse errors
		 * but we require history.
		 */
		history(cmd->line);
		free(cmd->line);
		cmd->line = NULL;
		return cmd;
	}
	history(cmd->line);

	return cmd;
err1:
	if (errno)
		perror(progname);

	return NULL;
}

void put_command (command_t *cmd)
{
	if (cmd->line)
		free(cmd->line);
	if (cmd->args) {
		while (cmd->arg_count) {
			free(cmd->args[cmd->arg_count - 1]);
			cmd->arg_count--;
		}
		free(cmd->args);
	}
	free(cmd);
}

void shell_loop (void)
{
	command_t * command;
	int do_exit = 0, i;

	debug(0, "Entering shell loop");
	while (!do_exit) {
		command = get_command();
		if (command == NULL) {
			do_exit = 1;
			continue;
		}
		if (!command->line)
			goto loop_again;

		printf("%s\n", command->args[0]);
		printf("%d\n", command->arg_count);
		i = 1;
		while (i < command->arg_count) {
			printf("argument %d: %s\n", i, command->args[i]);
			i++;
		}
	loop_again:
		put_command(command);
	}
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
