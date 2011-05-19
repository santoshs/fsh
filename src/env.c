#define _GNU_SOURCE
#include <stdio.h>
#include <search.h>
#include <execinfo.h>

#include "include/env.h"
#include "include/shell.h"

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
	{"HOME", NULL},
	{"INPUTRC", " "},
	{"TERM", "linux"},
};

static struct hsearch_data env;
static int env_iscreated;

int env_init (void)
{
	int i = 0;
	ENTRY ent, *ret;

	if (!hcreate_r(MAX_ENV_SIZE, &env)) {
		fprintf(stderr, "Cannot initialise the environment\n");
		return -1;
	}

	/*
	 * Set some defaults to a few important shell variables
	 */
	while (env_defaults[i].key) {
		if (setenv(env_defaults[i].key, env_defaults[i].value)) {
			fprintf(stderr, "Cannot set default environment for "
				"%s\n", progname);
			hdestroy_r(&env);
			return -1;
		}
		i++;
	}

	return 0;
}

char * getenv (char *var)
{
	ENTRY *ret;
	ENTRY ent;

	ent.key = var;
	ent.data = NULL;

	if (!hsearch_r(ent, FIND, &ret, &env)) {
		debug(LOG_DEBUG, TRACE_DEBUG, "Shell variable %s not set.\n",
		      ent.key);
		return NULL;
	}

	return (char *)ret->data;
}

int setenv (char *var, char *val)
{
	ENTRY *ret;
	ENTRY ent;

	ent.key = var;
	ent.data = (void *)val;

	if (!hsearch_r(ent, ENTER, &ret, &env)) {
		dump_stack();
		fprintf(stderr, "Cannot create new variable in environment\n");
		return -1;
	}

	return 0;
}