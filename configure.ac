AC_PREREQ([2.63])
AC_INIT([libntrace], [0.3.0], [info@bankai.nl])
AM_INIT_AUTOMAKE([-Wall subdir-objects])
AC_CONFIG_MACRO_DIR([m4])

AC_ARG_ENABLE([examples],
  AS_HELP_STRING([--enable-examples],[Build NTrace examples]),
  [case "${enableval}" in
     yes) examples=true;;
     no)  examples=false;;
     *) AC_MSG_ERROR([Bad value ${enableval} for --enable-examples]);;
  esac],
  [examples=false])
AM_CONDITIONAL([EXAMPLES], [test x$examples = xtrue])

m4_ifdef([AM_PROG_AR], [AM_PROG_AR])
AC_PROG_LIBTOOL
AC_PROG_CXX
AC_CHECK_HEADER([stdio.h],
  [AC_DEFINE([HAVE_STDIO_H], [1], [Define to 1 if you have <stdio.h>])],
  [AC_MSG_ERROR([sorry, can't do anything for you])])
AC_CHECK_HEADER([fnmatch.h],
  [AC_DEFINE([HAVE_FNMATCH], [1], [Define to 1 if you have <fnmatch.h>])],
  [AC_MSG_ERROR([Missing fnmatch.h from your system])])
AC_CHECK_HEADER([sys/time.h],
  [AC_DEFINE([HAVE_SYS_TIME_H], [1], [Define to 1 if you have <sys/time.h>])])

AC_CONFIG_FILES([
  Makefile examples/Makefile
])
AC_OUTPUT

