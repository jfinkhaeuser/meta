# SYNOPSIS
#
#   JF_SPLIT_VERSION
#
# DESCRIPTION
#
#   Splits a version number in the format MAJOR.MINOR into its
#   separate components.
#
#   Sets variables and defines.
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

AC_DEFUN([JF_SPLIT_VERSION],[
    AC_REQUIRE([AC_PROG_SED])
    JF_MAJOR_VERSION=`echo "$PACKAGE_VERSION" | $SED 's/\([[^.]][[^.]]*\).*/\1/'`
    JF_MINOR_VERSION=`echo "$PACKAGE_VERSION" | $SED 's/[[^.]][[^.]]*\.\([[^.]][[^.]]*\).*/\1/'`
    AC_MSG_CHECKING([Major version])
    AC_MSG_RESULT([$JF_MAJOR_VERSION])
    AC_DEFINE_UNQUOTED([PACKAGE_MAJOR], ["$JF_MAJOR_VERSION"], [Define to the
      major version of this package.])
    AC_MSG_CHECKING([Minor version])
    AC_MSG_RESULT([$JF_MINOR_VERSION])
    AC_DEFINE_UNQUOTED([PACKAGE_MINOR], ["$JF_MINOR_VERSION"], [Define to the
      MINOR version of this package.])
])
