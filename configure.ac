##############################################################################
# Setup
AC_PREREQ([2.68])
LT_PREREQ([2.4])

AC_INIT([meta], [0.1], [jens@unwesen.co.uk])
AC_CONFIG_SRCDIR([meta])
AM_INIT_AUTOMAKE
AC_CONFIG_HEADER([meta/_config.h])
AX_PREFIX_CONFIG_H([meta/meta-config.h])

LT_INIT
AC_SUBST([LIBTOOL_DEPS])
AC_CONFIG_MACRO_DIR([m4])

AC_LANG_CPLUSPLUS

##############################################################################
# Checks for programs.
AC_PROG_CXX
AC_PROG_GREP

##############################################################################
# Checks for compiler characteristics
AC_C_BIGENDIAN

# The AU_ALIAS is needed due to a bug in AX_CXX_HEADER_STDCXX_0X, etc.
AX_CXX_COMPILE_STDCXX_0X
AU_ALIAS([AC_COMPILE_STDCXX_0X], [AX_CXX_COMPILE_STDCXX_0X])

# Check if we have the null sentinel warning; it's annoying if we use it
# but don't have it.
JF_CHECK_COMPILE_FLAG([-Wstrict-null-sentinel])

# Check if the CXX mode was set on the configure command line
if echo "$CXXFLAGS" | $GREP -q 'DMETA_CXX_MODE=META_CXX_MODE_CXX0X' ; then
  jf_cxx_mode=0x
elif echo "$CXXFLAGS" | $GREP -q 'DMETA_CXX_MODE=META_CXX_MODE_CXX98' ; then
  jf_cxx_mode=98
else
  jf_cxx_mode=0x
fi
AM_CONDITIONAL([META_CXX_MODE_CXX0X], [test "$jf_cxx_mode" = 0x])
AM_CONDITIONAL([META_CXX_MODE_CXX98], [test "$jf_cxx_mode" = 98])

##############################################################################
# Checks for machine characteristics
# AX_CACHE_LINE_SIZE
# AX_COUNT_CPUS
# AX_CACHE_SIZE

##############################################################################
# Checks for libraries.
PKG_CHECK_MODULES([CPPUNIT], [cppunit >= 1.12])

##############################################################################
# Checks for header files.
AC_HEADER_STDC

AX_CXX_HEADER_STDCXX_98
AX_CXX_HEADER_STDCXX_TR1
AX_CXX_HEADER_STDCXX_0X

AC_CHECK_HEADERS([endian.h])
AC_CHECK_HEADERS([byteswap.h])
AC_CHECK_HEADERS([boost/date_time/posix_time/posix_time.hpp])


##############################################################################
# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_INT32_T
AC_TYPE_INT64_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T

##############################################################################
# Checks for library functions.
# AC_CHECK_FUNCS([gettimeofday])

##############################################################################
# Variable substitions
JF_SPLIT_VERSION

AC_SUBST([AM_CXXFLAGS])
AC_SUBST([AM_LDFLAGS])

##############################################################################
# Output
AC_CONFIG_FILES([
  Makefile
  meta/Makefile
  test/Makefile
  meta.pc
])
AC_OUTPUT
