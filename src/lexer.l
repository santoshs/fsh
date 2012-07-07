%{
#include "shell.h"
#include "parser.h"
%}

%%

[a-zA-z0-9\-\+\.\*\,\=\/\$\?]+	{ yylval.word = yytext; return WORD; }
[ \t]			{ ; }
L?\"(\\.|[^\\"])*\"	{ yylval.word = yytext + 1;
                          yylval.string[strlen(yylval.word) - 1] = '\0';
			  return WORD; }
\n			{ return EOL; }
;			{ return ';'; }
\|			{ return PIPE; }
&			{ return '&'; }
%%
