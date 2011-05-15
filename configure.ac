#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.68])
AC_INIT([shiritori], [1.0])
AM_INIT_AUTOMAKE([foreign])
AC_CONFIG_SRCDIR([server_main.cpp])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC

# Checks for libraries.
AC_CHECK_LIB([boost_program_options], [main])
if test ! "x$ac_cv_lib_boost_program_options_main" = xyes; then
  AC_MSG_ERROR("could not find boost_program_options library")
fi

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
