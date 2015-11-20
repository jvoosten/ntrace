AC_PREREQ([2.63])
AC_INIT([libntrace], [0.2.10], [info@bankai.nl])
AM_INIT_AUTOMAKE([-Wall])
AC_CONFIG_MACRO_DIR([m4])

AC_PROG_LIBTOOL
AC_PROG_CXX
AC_CHECK_HEADER([stdio.h],
  [AC_DEFINE([HAVE_STDIO_H], [1],
    [Define to 1 if you have <stdio.h>.])],
  [AC_MSG_ERROR([sorry, can do anything for you])])

AC_CONFIG_FILES([
  Makefile
])
AC_OUTPUT

