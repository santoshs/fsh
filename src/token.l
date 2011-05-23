%{
#include "parser.tab.h"
%}

%%
L?\"(\\.|[^\\"])*\" 	{ yylval.string = yytext + 1;
			  yylval.string[strlen(yylval.string) - 1] = '\0';
			  return STRING; }
[ \t]			{ ; }
[A-Za-z\-\+\.\/\*]+	{ yylval.string = yytext; return WORD; }
"<"			{ return REDIR_IN; }
">>"			{ return REDIR_OUT; }
">"			{ return REDIR_OUT_OW; }
"\|"			{ return PIPE; }
"&"			{ return AND; }
\n			{ return EOL; }
%%
