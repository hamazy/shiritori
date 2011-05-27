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
AC_CHECK_LIB([boost_thread], [main])
if test ! "x$ac_cv_lib_boost_thread_main" = xyes; then
  AC_MSG_ERROR("could not find boost_thread library")
fi

AC_CHECK_LIB([boost_program_options], [main])
if test ! "x$ac_cv_lib_boost_program_options_main" = xyes; then
  AC_MSG_ERROR("could not find boost_program_options library")
fi

AC_CHECK_LIB([boost_system], [main])
if test ! "x$ac_cv_lib_boost_system_main" = xyes; then
  AC_MSG_ERROR("could not find boost_system library")
fi

AC_CHECK_LIB([gtest_main], [main])
if test ! "x$ac_cv_lib_gtest_main_main" = xyes; then
  AC_MSG_ERROR("could not find gtest_main library")
fi

AC_CHECK_LIB([gtest], [main])
if test ! "x$ac_cv_lib_gtest_main" = xyes; then
  AC_MSG_ERROR("could not find gtest library")
fi

dnl AC_CHECK_LIB([gmock_main], [main])
dnl if test ! "x$ac_cv_lib_gmock_main_main" = xyes; then
dnl   AC_MSG_ERROR("could not find gmock_main library")
dnl fi

CXXFLAGS="-std=c++0x ${CXXFLAGS}"

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

AC_CONFIG_FILES([Makefile
                 test/Makefile])
AC_OUTPUT
