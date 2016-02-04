ACLOCAL_AMFLAGS = -I m4

bin_PROGRAMS = lklsocks

lklsocks_SOURCES = src/lklsocks.c++ \
                   src/lkl_helper.c \
                   src/lkl_decl.c \
                   src/asio/detail/impl/socket_ops.cpp \
                   src/SocksServer.c++ \
                   src/SocksConnection.c++

lklsocks_CPPFLAGS = $(BOOST_CPPFLAGS)
lklsocks_LDFLAGS = $(BOOST_PROGRAM_OPTIONS_LDFLAGS) $(BOOST_ASIO_LDFLAGS) $(BOOST_SYSTEM_LDFLAGS)
lklsocks_LDADD = $(BOOST_PROGRAM_OPTIONS_LIBS) $(BOOST_ASIO_LIBS) $(BOOST_SYSTEM_LIBS) $(vdeplug_LIBS)
