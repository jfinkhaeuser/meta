include $(top_srcdir)/common.mk

TESTS = testsuite
check_PROGRAMS = $(TESTS)

testsuite_CPPFLAGS = $(CPPUNIT_CFLAGS)
testsuite_LDADD = $(CPPUNIT_LIBS)

testsuite_SOURCES = \
	test_meta.cpp \
	test_mandatory.cpp \
	test_byteorder.cpp \
	testsuite.cpp
