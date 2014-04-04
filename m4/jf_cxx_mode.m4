# SYNOPSIS
#
#   JF_CXX_MODE
#
# DESCRIPTION
#
#   Derived from AX_CXX_COMPILE_STDCXX_0X
#
#   Check if the compiler with current compiler flags (i.e. what's passed on
#   the command line) compiles C++0x and also check if it compiles C++0x when
#   CXXFLAGS is temporarily cleared.
#
#   This isn't perfect, but it gives us some idea of whether passed CXXFLAGS
#   are forcing the compiler to go into c++98 mode:
#
#              CXXFLAGS intact    CXXFLAGS cleared
#   compiles:     c++11              c++11
#   errors:       c++98              c++98
#
# LICENSE
#
#   Author(s): Jens Finkhaeuser <jens@unwesen.co.uk>
#
#   Copyright (c) 2014 Unwesen Ltd.
#
#   This software is licensed under the terms of the GNU GPLv3 for personal,
#   educational and non-profit use. For all other uses, alternative license
#   options are available. Please contact the copyright holder for additional
#   information, stating your intended usage.
#
#   You can find the full text of the GPLv3 in the COPYING file in this code
#   distribution.
#
#   This software is distributed on an "AS IS" BASIS, WITHOUT ANY WARRANTY;
#   without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
#   PARTICULAR PURPOSE.

#serial 1

AC_DEFUN([JF_CXX_MODE], [
  AC_CACHE_CHECK(if compiler supports C++0x features with the passed CXXFLAGS,
  jf_cv_cxx_mode_withflags,
  [AC_LANG_SAVE
  AC_LANG_CPLUSPLUS
  AC_TRY_COMPILE([
  template <typename T>
    struct check
    {
      static_assert(sizeof(int) <= sizeof(T), "not big enough");
    };

    typedef check<check<bool>> right_angle_brackets;

    int a;
    decltype(a) b;

    typedef check<int> check_type;
    check_type c;
    check_type&& cr = static_cast<check_type&&>(c);],,
  jf_cv_cxx_mode_withflags=yes, jf_cv_cxx_mode_withflags=no)
  AC_LANG_RESTORE
  ])

  AC_CACHE_CHECK(if compiler supports C++0x features withOUT the passed CXXFLAGS,
  jf_cv_cxx_mode_withoutflags,
  [AC_LANG_SAVE
  jf_cv_cxx_mode_saveflags=$CXXFLAGS
  CXXFLAGS=
  AC_LANG_CPLUSPLUS
  AC_TRY_COMPILE([
  template <typename T>
    struct check
    {
      static_assert(sizeof(int) <= sizeof(T), "not big enough");
    };

    typedef check<check<bool>> right_angle_brackets;

    int a;
    decltype(a) b;

    typedef check<int> check_type;
    check_type c;
    check_type&& cr = static_cast<check_type&&>(c);],,
  jf_cv_cxx_mode_withoutflags=yes, jf_cv_cxx_mode_withoutflags=no)
  AC_LANG_RESTORE
  CXXFLAGS=$jf_cv_cxx_mode_saveflags
  ])

  jf_cv_cxx_mode_cxx98=0xdeadbeef00001998LL
  jf_cv_cxx_mode_cxx0x=0xdeadbeef00002011LL

  jf_cv_cxx_mode_result=$jf_cv_cxx_mode_cxx98
  if test "$jf_cv_cxx_mode_withflags" = yes ||
     test "$jf_cv_cxx_mode_withoutflags" = yes; then
    jf_cv_cxx_mode_result=$jf_cv_cxx_mode_cxx0x
  elif test "$jf_cv_cxx_mode_withflags" = no ||
       test "$jf_cv_cxx_mode_withoutflags" = no; then
    jf_cv_cxx_mode_result=$jf_cv_cxx_mode_cxx98
  fi
  AC_DEFINE_UNQUOTED([CXX_MODE_CXX98], [$jf_cv_cxx_mode_cxx98], [C++ macro for c++98 mode])
  AC_DEFINE_UNQUOTED([CXX_MODE_CXX0X], [$jf_cv_cxx_mode_cxx0x], [C++ macro for c++11 mode])
  AC_DEFINE_UNQUOTED([CXX_MODE], [$jf_cv_cxx_mode_result], [C++ mode used by the compiler.])

  AM_CONDITIONAL([CXX_MODE_CXX98], [test "$jf_cv_cxx_mode_result" == "$jf_cv_cxx_mode_cxx98"])
  AM_CONDITIONAL([CXX_MODE_CXX0X], [test "$jf_cv_cxx_mode_result" == "$jf_cv_cxx_mode_cxx0x"])
])
