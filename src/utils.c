#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <execinfo.h>
#include <malloc.h>
#include <syslog.h>
#include <stdarg.h>

#include "env.h"
#include "shell.h"

/*
 * Whitespace cleanup
 */
char *strip (char *str)
{
	register char *s, *t;

	for (s = str; isspace (*s); s++)
		;

	if (*s == 0)
		return (s);

	t = s + strlen (s) - 1;
	while (t > s && isspace (*t))
		t--;
	*++t = '\0';

	return s;
}

void dump_stack (void)
{
	void *array[10];
	size_t size;
	char **strings;
	size_t i;

	size = backtrace (array, 10);
	strings = backtrace_symbols (array, size);

	printf ("Obtained %zd stack frames.\n", size);

	for (i = 0; i < size; i++)
		printf ("%s\n", strings[i]);

	free (strings);
}

void inline
slog (int priority, char flags, char *s, ...)
{
	va_list vlist;
	if ((flags & TRACE_DEBUG) || (flags_global & TRACE_DEBUG)) {
		dump_stack();
	}

	va_start(vlist, s);
	vsyslog(priority, s, vlist);
	va_end(vlist);
}
