# Process this file with autoconf to produce a configure script.
AC_PREREQ([2.68])
AC_INIT([vendor-project-name], [0.0.1], [bugfixes@support-email.com])

AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([m4])

# location of additional build machinary
AC_CONFIG_HEADERS([config.h])
AC_SUBST([ACLOCAL_AMFLAGS], ["-I $ac_macro_dir \${ACLOCAL_FLAGS} --install"])
AC_CONFIG_SRCDIR([Makefile.am])
# setup automake
AM_INIT_AUTOMAKE([1.10 no-define foreign tar-pax subdir-objects])

# enable maintainer mode
AM_MAINTAINER_MODE
# allow for silent builds in older version of automake 12.04 lts cough
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES])

dnl disable unnecessary compiler checks
AC_DEFUN([AC_PROG_F77], [:])
AC_DEFUN([AC_PROG_FC], [:])
AC_DEFUN([AC_PROG_OBJC], [:])
AC_DEFUN([AC_PROG_OBJCCPP], [:])
AC_DEFUN([AC_LIBTOOL_F77], [:])

# use platform extensions
AC_USE_SYSTEM_EXTENSIONS
AX_CXX_COMPILE_STDCXX_11([noext],[mandatory])
# Checks for programs.
AC_PROG_CPP
AC_PROG_CC
AC_PROG_CXX
AC_PROG_GREP
AC_PROG_SED
AC_PROG_AWK
AC_PROG_INSTALL
AC_PROG_LN_S
dnl prefer gmake to bsd make
AC_CHECK_PROGS([MAKE], [gmake make])
AC_PROG_MKDIR_P
AC_PROG_LIBTOOL
AC_MINGW32
# check if we can format code nicely
FUMA_AX_ASTYLE([2.03])
# Check if we can use compiler caching
FUMA_AX_CCACHE
# check if we can do code coverage
AC_TDD_GCOV
# workout what platform we are building for
FUMA_AX_CANONICAL_HOST

# enable debug builds
FUMA_AX_DEBUG

# check for platform minimal requirments
FUMA_AX_PLATFORM_TYPES
# Checks for header files.
AC_HEADER_STDBOOL
AC_HEADER_TIME
AC_HEADER_STDC
AC_CHECK_HEADERS([limits.h execinfo.h ucontext.h])
# find the platform threading library
#
# fixme: mingw requires -mthreads in PTHREAD_CFLAGS
AX_PTHREAD
# check if we can find boost
AX_BOOST_BASE([1.58],, [AC_MSG_ERROR([Boost was not found in your system])])
AX_BOOST_SYSTEM
AX_BOOST_PROGRAM_OPTIONS
AX_BOOST_UNIT_TEST_FRAMEWORK
AX_BOOST_REGEX
AX_BOOST_FILESYSTEM
AX_BOOST_IOSTREAMS
# see about postgres
#FUMA_AX_POSTGRES([9.4.0], [],[AC_MSG_ERROR([PostgreSQL was not found in your system])])
# find the version of webtoolkit libraries
FUMA_AX_WEBTOOLKIT([3.3.2],[],[
        AC_ERROR([Could not find suitable version of libWt])
])
# generate the output

AC_CONFIG_FILES([Makefile
                 sources/Makefile
                 sources/include/Makefile
                 sources/lib/Makefile
                 sources/src/Makefile
                 tests/Makefile
                 tests/include/Makefile
                 tests/lib/Makefile
                 tests/src/Makefile])

AC_OUTPUT
