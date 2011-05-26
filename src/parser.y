%{
#include <stdio.h>
#include <string.h>
#include <readline/readline.h>

#include "shell.h"

#define YYERROR_VERBOSE

command_t *command_ptr;

command_t * make_cmd_arg (char *, command_t *);
void yyerror(char const *);
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

	debug(0, "Entering shell loop");
	while (line = readline(getprompt())) {
		yy_scan_string(line);
		yyparse();
		t = strip(line);
		if (*t)
			add_history(t);
		yyrestart();
		free(line);
		line = NULL;
	}
}

void yyerror(char const *s)
{
	if (command_ptr) {
		put_command(command_ptr);
		command_ptr = NULL;
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
		INIT_LIST_HEAD(&t->args.list);
		/*
		 * save the current command_t pointer globally, so it
		 * can be freed when a parse error is encountered
		 */
		command_ptr = t;
	}

	args = malloc(sizeof(args_t));
	args->arg = strdup(s);
	list_add_tail(&args->list, &t->args.list);
	return t;
}
