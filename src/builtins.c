#include "shell.h"

void *cmd_builtin_cd (command_t *cmd)
{
	chdir(cmd->argv[1]);
	return (void *) 0;
}

builtin_t builtin[] = {
	{
		.cmd_str = "cd",
		.builtin_cb = cmd_builtin_cd,
	},
	NULL,
};
