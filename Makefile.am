ACLOCAL_AMFLAGS = -I m4

bin_PROGRAMS = lklsocks

lklsocks_SOURCES = src/lklsocks.c++ \
                   src/lkl_helper.c

lklsocks_CPPFLAGS = $(BOOST_CPPFLAGS)
lklsocks_LDFLAGS = $(BOOST_PROGRAM_OPTIONS_LDFLAGS)
lklsocks_LDADD = $(BOOST_PROGRAM_OPTIONS_LIBS) $(vdeplug_LIBS)