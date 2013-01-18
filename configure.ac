#                  -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.66])
AC_INIT([fshell - my toy shell], [0.1], [ssantosh@fossix.org], [fshell])
AM_INIT_AUTOMAKE(fshell, 0.5)
AC_CONFIG_SRCDIR([config.h.in])
AC_CONFIG_HEADERS([config.h])

AC_ARG_ENABLE([debug],
     [  --enable-debug    Turn on debugging],
     [case "${enableval}" in
       yes) debug=true ;;
       no)  debug=false ;;
       *) AC_MSG_ERROR([bad value ${enableval} for --enable-debug]) ;;
       esac],[debug=false])
          AM_CONDITIONAL([CONFIG_DEBUG], [test x$debug = xtrue])

# Checks for programs.
AC_PROG_CC
AC_PROG_LEX
AC_PROG_YACC

# Checks for libraries.
AC_CHECK_LIB([readline], [readline], [
              AC_SUBST([READLINE_LIBS], -lreadline)
              AC_DEFINE([HAVE_LIBREADLINE], [1], [Define to 1 if you have the `readline' library (-lreadline).])
            ])

AC_CHECK_LIB([flex], [fl], [
              AC_SUBST([LEXLIBS], -lfl)
              AC_DEFINE([HAVE_LIBFLEX], [1], [Define to 1 if you have the `flex' library (-lfl).])
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
