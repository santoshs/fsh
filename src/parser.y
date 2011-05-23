%{
#include <syslog.h>

#include "parsing.h"
%}
%union {
	char *string;
	char ch;
}

%token <string> STRING
%token <string> WORD
%token REDIR_IN REDIR_OUT REDIR_OUT_OW PIPE
%token EOL AND

%%

input_line:                     /* blank line*/
 | command { ; }
 | command AND { ; }
 | command PIPE command { ; }
 | command STRING { printf("%s\n", $2); }
 | command REDIR_IN WORD { ; }
 | command REDIR_OUT WORD { ; }
 | command REDIR_OUT_OW WORD { ; } 
 ;
command: WORD
 | command WORD {  }
 ;
%%

void yyerror(char const *err)
{
        slog(LOG_ERR, 0, "%s\n", err);
}
