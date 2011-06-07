#ifndef _SHELL_H_
#define _SHELL_H_

#include <syslog.h>
#include <stdlib.h>

#include "list.h"		/* Taken from the Linux Kernel */
#define MAX_WORD_LENGTH		256
#define MAX_BUILTINS		2

/* Global flags */
#define BASIC_DEBUG		0
#define TRACE_DEBUG		1

/* Constants */
#define MIN_ARG_ALLOC		8
#define MIN_ARG_ALLOC_INC	8

/* Command flags */
#define BACKGROUND_JOB		0x0001
#define REDIR_OVEREWRITE	0x0002

typedef struct args_ {
	struct list_head list;
	char *arg;
} args_t;

typedef struct command_ {
	struct list_head list;
	args_t arg_list;
	int arg_count;
	char **argv;
	char *line;
	char *redir_in;
	char *redir_out;
	pid_t pid;
	unsigned short flags;
} command_t;

typedef struct builtin_ {
	struct list_head list;
	char *cmd_str;
	int (*builtin_cb)(command_t *);
} builtin_t;

typedef struct jobs_ {
	struct list_head list;
	command_t *job;
	unsigned short jid;
	pid_t pid;		/* pid of the last command */
	char *line;
} jobs_t;

extern command_t *current;		/* current executing job */
extern jobs_t jobs;			/* List of background jobs */
extern char *global_line;

enum job_status {
	JOB_DONE,
	JOB_BACKGROUND,
	JOB_FAIL,
	NOT_FOUND,
	EXIT,
};

extern unsigned int flags_global;
extern char *progname;

char *strip(char *);
void inline slog(int, char, char *, ...);
void put_command(command_t *);
char *getprompt(void);

/* convinient macros for debugging and logging */
#define debug(flags, x...) slog(LOG_DEBUG, flags, ##x)
#define fatal(flags, x...) do {			\
		slog(LOG_CRIT, flags, ##x);	\
		exit(EXIT_FAILURE);		\
	} while(0)

/* count the number of list elements */
static inline int list_count(struct list_head *l)
{
	struct list_head *p;
	int i = 0;
	if (list_empty(l))
		return 0;
	list_for_each(p, l)
		i++;
	return i;
}
#endif /* _SHELL_H_ */
