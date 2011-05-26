%{
#include <stdio.h>
#include <string.h>
#include <readline/readline.h>

#include "shell.h"

#define YYERROR_VERBOSE

command_t * make_cmd_arg (char *, command_t *);
void yyerror(char const *);

command_t *current;
jobs_t jobs;
char *global_line;
%}

%token WORD EOL PIPE

%union {
	char *word;
	char *string;
	char c;
	command_t *command;
};

%type <command> line command
%%

line:			{ ; }
 | command		{ do_command($1); YYACCEPT; }
 ;
command: WORD		{ $$ = make_cmd_arg(yylval.word, NULL); }
 | command '&'		{ $$->flags |= BACKGROUND_JOB; }
 | command WORD		{ $$ = make_cmd_arg(yylval.word, $1); }
 | command PIPE command	{ ; }
 ;
%%

void shell_loop (void)
{
	char *line, *t;
	jobs_t *j;
	struct list_head *p, *s;
	int state;
	pid_t pid;

	debug(0, "Entering shell loop");
	while (line = readline(getprompt())) {
		yy_scan_string(line);
		global_line = line;
		yyparse();
		yyrestart();
		t = strip(line);
		if (*t)
			add_history(t);
		free(line);
		line = NULL;

		if (!current || current->flags & BACKGROUND_JOB) {
			current = NULL;
			continue;
		} else {
			put_command(current);
			current = NULL;
		}

		pid = waitpid(0, &state, WNOHANG);
		list_for_each_safe(p, s, &jobs.list) {
			j = list_entry(p, jobs_t, list);
			if (pid == j->pid) {
				list_del(p);
				fprintf(stderr, "[%d - Done] %s\n", j->pid,
					j->line);
				free(j->line);
				put_command(j->job);
				free(j);
			}
		}
	}
}

void yyerror(char const *s)
{
	if (current) {
		put_command(current);
		current = NULL;
	}
	fprintf(stderr, "%s\n", s);
}

command_t * make_cmd_arg (char *s, command_t *cmd)
{
	command_t *t = cmd;
	args_t *args;

	if (!s || ! *s) {
		return NULL;
	}

	if (!t) {
		t = malloc(sizeof(command_t));
		if (!t)
			fatal(BASIC_DEBUG, "Memory allocation failure\n");
		memset(t, 0, sizeof(command_t));
		INIT_LIST_HEAD(&t->list);
		INIT_LIST_HEAD(&t->arg_list.list);
		/*
		 * save the current command_t pointer globally, so it
		 * can be freed when a parse error is encountered
		 */
		current = t;
	}

	args = malloc(sizeof(args_t));
	args->arg = strdup(s);
	list_add_tail(&args->list, &t->arg_list.list);
	t->arg_count++;
	return t;
}
