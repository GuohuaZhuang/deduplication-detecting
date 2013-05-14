#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.59])
AC_INIT([dup], [1.0])
AC_CONFIG_SRCDIR([src/dup.h])
AM_INIT_AUTOMAKE()

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_LIBTOOL

# Checks for libraries.
# PKG_CHECK_EXISTS([glib-2.0], [], [
#     AC_MSG_ERROR(["Error! You need to have libglib2.0 installed."])
#     ])

# PKG_CHECK_EXISTS([cppunit], [], [
#     AC_MSG_ERROR(["Error! You need to have libcppunit-dev installed. Try sudo apt-get install libcppunit-dev."])
#     ])

# I don't know how to check the crfpp lib and head file whether is exits in automake, but I know the CMake do 
# following is good to do it.(reference in nlpbamboo-1.1.2)
# 
# PKG_CHECK_EXISTS([crfpp], [], [
#     AC_MSG_ERROR(["Error! You need to have crfpp installed. Try to read the INSTALL file."])
#     ])

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h stdio.h])
AC_CHECK_HEADERS([malloc.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_TYPE_SIZE_T

# check dup debug status.
AC_ARG_WITH(debug,
    [AS_HELP_STRING([--with-debug], [enable debugging output])],
    [
        if test $withval != "no"; then
            AC_SUBST(AM_CFLAGS, ["-g3 -O0 -Wall"])
            AC_SUBST(AM_CXXFLAGS, ["-g3 -O0 -Wall"])
            AC_DEFINE([WITH_DEBUG],1)
        fi
    ])

# check dup sysconfig dir.
AC_ARG_WITH(conf,
    [AS_HELP_STRING([--with-conf], [set dup sysconfig path where dup conf file is locate.])],
    [
        if test $withval != "no"; then
            AC_SUBST(dup_sysconfig_dir, $withval)
        fi
    ])
AS_IF([test "x$dup_sysconfig_dir" = "x"],
        [
            dup_sysconfig_dir="/etc/scholar2"
            AC_SUBST(dup_sysconfig_dir)
    ])

# check dup data dir.
AC_ARG_WITH(data,
    [AS_HELP_STRING([--with-data], [set dup data path where simhash table file is locate.])],
    [
        if test $withval != "no"; then
            AC_SUBST(dup_data_dir, $withval)
        fi
    ])
AS_IF([test "x$dup_data_dir" = "x"],
        [
            dup_data_dir="/ape/scholar2"
            AC_SUBST(dup_data_dir)
    ])

AC_SUBST(dup_sysconfig_dir)
AC_SUBST(dup_data_dir)
AC_SUBST(AM_CPPFLAGS)
AC_SUBST(AM_LDFLAGS)
AC_SUBST(CFLAGS, "")
AC_SUBST(CPPFLAGS, "")
AC_SUBST(INSTALL_DATA, "${INSTALL} -m 777")
AC_SUBST(MKDIR_P, "/bin/mkdir -p -m 777")

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([memset])
AC_CHECK_FUNCS([strchr])
AC_CHECK_FUNCS([strstr])
AC_CHECK_FUNCS([gettimeofday])
AC_CHECK_FUNCS([pow])
AC_CHECK_HEADERS([limits.h])
AC_CHECK_HEADERS([sys/param.h])
AC_CHECK_HEADERS([sys/time.h])
AC_C_INLINE
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_TYPE_UINT8_T
AC_C_CONST

AC_DEFINE(_LARGEFILE64_SOURCE)
AC_DEFINE(_XOPEN_SOURCE, [500])
AC_DEFINE(_FILE_OFFSET_BITS, [64])
AC_DEFINE(_GNU_SOURCE)

AC_CONFIG_FILES([Makefile
                 src/Makefile
                 conf/Makefile
                 data/Makefile])

AC_OUTPUT

