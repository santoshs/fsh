#ifndef _SHELL_H_
#define _SHELL_H_

#include <syslog.h>

#include "list.h"		/* Taken from the Linux Kernel */
#define MAX_WORD_LENGTH		256

/* Global flags */
#define BASIC_DEBUG		0
#define TRACE_DEBUG		1

/* Constants */
#define MIN_ARG_ALLOC		8
#define MIN_ARG_ALLOC_INC	8

typedef struct command_ {
	struct list_head list;
	char **args;
	int arg_count;
	char *line;
	char *redir_in;
	char *redir_out;
} command_t;

struct builtin_ {
	struct list_head list;
	char *cmd_str;
	void *(*builtin_cb)(command_t *);
};

enum job_status {
	JOB_DONE,
	JOB_BACKGROUND,
	JOB_FAIL,
	NOT_FOUND,
	EXIT,
};

extern unsigned int flags_global;

char *strip(char *);
void inline slog(int, char, char *, ...);
void put_command (command_t *);

/* convinient macros for debugging and logging */
#define debug(flags, x...) slog(LOG_DEBUG, flags, ##x)
#define fatal(flags, x...) do {			\
		slog(LOG_CRIT, flags, ##x);	\
		exit(EXIT_FAILURE);		\
	} while(0)
#endif /* _SHELL_H_ */
