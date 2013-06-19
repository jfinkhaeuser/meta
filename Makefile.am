ACLOCAL_AMFLAGS = -I m4

LIBTOOL_DEPS = @LIBTOOL_DEPS@
libtool: $(LIBTOOL_DEPS)
	$(SHELL) ./config.status libtool

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = meta.pc

SUBDIRS = \
	meta \
	test
