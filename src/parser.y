%{
#include <stdio.h>
#include <readline/readline.h>

#include "shell.h"

command_t * make_cmd_arg (char *s, command_t *cmd);
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
 | line PIPE command	{ printf("pipe commands\n"); }
 ;
command: WORD		{ $$ = make_cmd_arg(yylval.word, NULL); }
 | command '&'		{ $$->flags |= BACKGROUND_JOB; }
 | command WORD		{ $$ = make_cmd_arg(yylval.word, $1); }
 ;
%%

int main (void)
{
	char *line;

	while (line = readline("$ ")) {
		yy_scan_string(line);
		yyparse();
		yyrestart();
		free(line);
		line = NULL;
	}
	return 0;
}

void yyerror(char const *s)
{
	fprintf(stderr, "%s\n", s);
}

command_t * make_cmd_arg (char *s, command_t *cmd)
{
	printf("%s\n", s);
	return NULL;
}
