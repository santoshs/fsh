bin_PROGRAMS = sshell
sshell_SOURCES = shell.c env.c parser.y token.l utils.c shell.h env.h
LDADD = -lreadline -lfl

clean:
	rm -f parser.tab.c parser.tab.h token.c *.o $(bin_PROGRAMS)
