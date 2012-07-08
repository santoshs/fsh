#include <stdio.h>
#include <execinfo.h>
#include <sys/types.h>
#include <pwd.h>

#include "env.h"
#include "shell.h"

extern char *progname;

/*
 * Default environment variables
 *
 * These are mostly set here as default for the readline library.
 * Readline expects some enviroment variables, setting it so I can
 * expect it to work better.
 *
 * NULL in the value means, we need to set it by querying elsewhere.
 */
aarray_t env_defaults[] = {
	{"PS1", "$ "},
	{"PS2", "> "},
	{"LANG", "en_IN.utf8"},	/* This needs to be set dynamically */
	{"INPUTRC", ""},
	{"TERM", "linux"},
	{"SHELL", "sshell"},
	{},
};

int env_init (void)
{
	int i = 0;
	struct passwd *pw;

	/*
	 * Set some defaults to a few important shell variables
	 */
	while (env_defaults[i].key) {
		setenv(env_defaults[i].key, env_defaults[i].value, 1);
		i++;
	}

	/* Set the users home directory */
	pw = getpwuid(getuid());
	setenv("HOME", pw->pw_dir, 1);

	return 0;
}
