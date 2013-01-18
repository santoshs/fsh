bin_PROGRAMS = fsh
fsh_SOURCES = shell.c env.c parser.c lexer.l utils.c shell.h env.h builtins.c
LEXLIB = @LEXLIB@
LDADD = @READLINE_LIBS@ $(LEXLIB)

if CONFIG_DEBUG
AM_CFLAGS = -ggdb -DCONFIG_DEBUG
AM_YFLAGS = -t
endif

YACC_SOURCE = parser.y
YACC_HEADER = $(YACC_SOURCE:.y=.h)
YACC_OUTPUT = $(YACC_SOURCE:.y=.c)

$(YACC_OUTPUT): $(YACC_SOURCE)
	bison --defines=$(YACC_HEADER) $(AM_YFLAGS) -o $@ $^
clean:
	rm -f parser.c parser.h lexer.c *.o $(bin_PROGRAMS)
