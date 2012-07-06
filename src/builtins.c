#include "shell.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

int builtin_cd (command_t *cmd)
{
	chdir(cmd->argv[1]);
	return 0;
}

int builtin_echo (command_t *cmd)
{
	int oflags = 0, fd = 1, i;
	char *arg;

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
		arg = cmd->argv[i];
		if (arg[0] == '$')
			/* Request for an environment variable */
			arg = getenv(&arg[1]);

		if (arg) {
			write(fd, arg, strlen(arg));
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
