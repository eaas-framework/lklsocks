dnl Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT(lklsocks, 0.1, http://github.com/eaas-framework/lklsocks)
AC_CONFIG_MACRO_DIR([m4])

AM_INIT_AUTOMAKE([subdir-objects foreign])
AM_SILENT_RULES([yes])

AC_PROG_LIBTOOL
AC_PROG_CXX
AC_PROG_CC

AX_CXX_COMPILE_STDCXX_11(noext)

LIBS="-lpthread -lrt -ldl"

PKG_CHECK_MODULES([vdeplug], [vdeplug])
LIBS+=" $vdeplug_LIBS"

AC_LANG_PUSH([C])
AC_CHECK_HEADERS([lkl.h lkl_host.h],
			     [],
			     AC_MSG_ERROR([The Linux Kernel Library headers could not be found.]),
				 [#undef st_atime
				  #undef st_mtime
				  #undef st_ctime])
AC_SEARCH_LIBS([lkl_sys_halt], [lkl], [], AC_MSG_ERROR([The Linux Kernel Library objects could not be found.]))
AC_LANG_POP([C])

BOOST_REQUIRE([1.58])
BOOST_SYSTEM([s])
BOOST_PROGRAM_OPTIONS([s])
BOOST_ASIO([s])


AC_CONFIG_FILES(Makefile)
AC_OUTPUT

