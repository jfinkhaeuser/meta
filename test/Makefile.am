include $(top_srcdir)/common.mk

TESTS = testsuite
check_PROGRAMS = $(TESTS)

testsuite_CPPFLAGS = $(CPPUNIT_CFLAGS)
testsuite_LDADD = $(CPPUNIT_LIBS)

testsuite_SOURCES = \
	test_mandatory.cpp \
	test_meta.cpp \
	test_byteorder.cpp \
	test_pointers.cpp \
	test_math.cpp \
	testsuite.cpp

if META_CXX_MODE_CXX0X
testsuite_SOURCES += \
	test_condition.cpp \
	test_restricted.cpp \
	test_singleton.cpp \
	test_lists.cpp \
	test_range.cpp
endif
