#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
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
	add_history(line);
}

command_t * get_command (void)
{
	command_t *cmd;
	static char *line;
	char *tmp;

	if (line) {
		free(line);
		line = NULL;
	}
	if ((line = readline(getprompt())) == NULL) {
		goto err1;
	}

	cmd = malloc(sizeof(command_t));
	if (!cmd) {
		goto err1;
	}
	tmp = strip(line);
	cmd->line = strdup(tmp);
	if (!cmd->line)
		goto err2;

	if (parse_line(cmd)) {
		fprintf(stderr, "%s: Parse error\n", progname);
		/*
		 * Not going to exit shell for parse errors
		 * but we require history.
		 */
		history(cmd->line);
		free(cmd->line);
		cmd->line = NULL;
		return cmd;
	}

	return cmd;
err3:
	free(cmd->line);
err2:
	free(cmd);
err1:
	if (errno)
		perror(progname);

	return NULL;
}

void shell_loop (void)
{
	command_t * command;
	int do_exit = 0;

	while (!do_exit) {
		command = get_command();
		if (command == NULL) {
			do_exit = 1;
			continue;
		}
		if (!command->line)
			goto loop_again;

		printf("%s\n", command->line);
		history(command->line);
	loop_again:
		free(command->line);
		free(command);
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
