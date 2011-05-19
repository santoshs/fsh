#ifndef _SHELL_H_
#define _SHELL_H_

#include <syslog.h>
#define MAX_WORD_LENGTH		256

/* Global flags */
#define TRACE_DEBUG		1

typedef struct command_ {
	struct command_ *next;
	char **args;
	int count;
	char *line;
	char *redir_in;
	char *redir_out;
} command_t;

extern unsigned int flags_global;

char *strip(char *);
void inline debug(int, char, char *, ...);


#endif /* _SHELL_H_ */
