%{
#include <stdio.h>

#include "shell.h"

enum yytokentype {
	NUMBER = 258,
	WORD,
	STRING,
	CHAR,
};

int number;
char *arg;
char c;
%}

%%
L?\"(\\.|[^\\"])*\" 	{ arg = yytext + 1; arg[strlen(arg) - 1] = '\0';
			  return STRING; }
[ \t]			{ ; }
[A-Za-z\-\+\.\/\*]+		{ arg = yytext; return STRING; }
%%

// " this quote is for proper font-locking, it does not yet know lex
// regexp properly which is present above

int parse_line (command_t *cmd, char *line)
{
	enum yytokentype tok; char *tmp;
	int i = 0;

	tmp = strip(line);

	if (!*tmp)
		return -1;

	cmd->line = strdup(tmp);
	if (!cmd->line)
		goto err;

	/* Allocating memory for MIN_ARG_ALLOC */
	cmd->args = malloc(sizeof(char *) * MIN_ARG_ALLOC);
	if (!cmd->args)
		goto err1;
	cmd->arg_count = MIN_ARG_ALLOC;

	yy_scan_string(tmp);
	while (tok = yylex()) {
		switch(tok) {
		case STRING:
			cmd->args[i] = strdup(arg);
			i++;
			break;
		default:
			put_command(cmd);
			return 1; /* Parse error, shouldn't happen */
		}
	}
	cmd->arg_count = i;
	return 0;
err1:
	free(cmd->line);
err:
	fatal(TRACE_DEBUG, "Memory allocation failed\n"); /* No return */
}
