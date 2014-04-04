# SYNOPSIS
#
#   JF_CHECK_COMPILE_FLAG
#
# DESCRIPTION
#
#   See AX_CHECK_COMPILE_FLAG
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

AC_DEFUN([JF_CHECK_COMPILE_FLAG], [
    AC_REQUIRE([AX_CHECK_COMPILE_FLAG])

    jf_check_compile_flag_value=
    AX_CHECK_COMPILE_FLAG([$1], [
      $2
      jf_check_compile_flag_value="1"
    ], [$3], [-Werror $4])

    if test "$jf_compile_flag_value" == "$1" ; then
      AC_DEFINE_UNQUOTED(m4_toupper(m4_translit(HAVE_FLAG_$1, [-], [_])),
        [$jf_check_compile_flag_value],
        [Compiler understands $1 flag.])
    fi

    AM_CONDITIONAL(m4_toupper(m4_translit(HAVE_FLAG_$1, [-], [_])),
      [test "$jf_check_compile_flag_value" == "1"])
])
