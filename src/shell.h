#ifndef _SHELL_H_
#define _SHELL_H_

#include <syslog.h>
#define MAX_WORD_LENGTH		256

/* Global flags */
#define BASIC_DEBUG		0
#define TRACE_DEBUG		1

/* Constants */
#define MIN_ARG_ALLOC		8
#define MIN_ARG_ALLOC_INC	8

typedef struct command_ {
	struct command_ *next;
	char **args;
	int arg_count;
	char *line;
	char *redir_in;
	char *redir_out;
} command_t;

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
