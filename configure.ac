AC_PREREQ([2.63])
AC_INIT([libntrace], [0.3.0], [info@bankai.nl])
AM_INIT_AUTOMAKE([-Wall subdir-objects])
AC_CONFIG_MACRO_DIR([m4])

m4_ifdef([AM_PROG_AR], [AM_PROG_AR])
AC_PROG_LIBTOOL
AC_PROG_CXX
AC_CHECK_HEADER([stdio.h],
  [AC_DEFINE([HAVE_STDIO_H], [1],
    [Define to 1 if you have <stdio.h>.])],
  [AC_MSG_ERROR([sorry, can't do anything for you])])
AC_CHECK_HEADER([fnmatch.h],
  [AC_DEFINE([HAVE_FNMATCH], [1],
    [Define to 1 if you have <fnmatch.h>])],
  [AC_MSG_ERROR([Missing fnmatch.h from your system])])
AC_CONFIG_FILES([
  Makefile
])
AC_OUTPUT

