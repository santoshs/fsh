#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.66])
AC_INIT([sshell - my toy shell], [0.1], [santoshs@fossix.org], [sshell])
AM_INIT_AUTOMAKE(sshell, 0.5)
AC_CONFIG_SRCDIR([config.h.in])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC
AC_PROG_LEX

# Checks for libraries.
AC_CHECK_LIB([readline], [readline], [
              READLINE_LIBS=-lreadline
              AC_DEFINE([HAVE_LIBREADLINE], [1], [Define to 1 if you have the `readline' library (-lreadline).])
            ])

# Checks for header files.
AC_CHECK_HEADERS([malloc.h string.h syslog.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_INLINE
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([setenv strdup])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT([src/Makefile])
