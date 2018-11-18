#include "shell.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

int builtin_cd (command_t *cmd)
{
	chdir(cmd->argv[1]);
	return 0;
}

extern int last_exit;
int builtin_echo (command_t *cmd)
{
	int oflags = 0, fd = 1, i;
	char *arg, *str;
	char temp[100];

	if (cmd->redir_out) {
		if (cmd->flags & REDIR_OVEREWRITE)
			oflags |= O_TRUNC;
		else
			oflags |= O_APPEND;
		fd = open(cmd->redir_out, oflags | O_CREAT);
		if (fd < 0) {
			perror(progname);
			return errno;
		}
	}

	for (i = 1; i < cmd->arg_count; i++) {
		str = arg = cmd->argv[i];
		if (arg[0] == '$') {
			/* Request for an environment variable */
			str = getenv(&arg[1]);
			if (!str) {
				/* Oops! no env, something else? */
				if (arg[1] == '?') {
					sprintf(temp, "%d", last_exit);
					str = temp;
				}
			}
		}
		if (str) {
			write(fd, str, strlen(str));
			write(fd, " ", strlen(" "));
		}
	}
	write(fd, "\n", strlen("\n"));

	return 0;
}

builtin_t builtin[] = {
	{
		.cmd_str = "cd",
		.builtin_cb = builtin_cd,
	},
	{
		.cmd_str = "echo",
		.builtin_cb = builtin_echo,
	},
};
