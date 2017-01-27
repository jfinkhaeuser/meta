#!/bin/bash
#
# This file is part of meta.
#
# Author(s): Jens Finkhaeuser <jens@finkhaeuser.de>
#
# Copyright (c) 2017 Jens Finkhaeuser
#
# This software is licensed under the terms of the GNU GPLv3 for personal,
# educational and non-profit use. For all other uses, alternative license
# options are available. Please contact the copyright holder for additional
# information, stating your intended usage.
#
# You can find the full text of the GPLv3 in the COPYING file in this code
# distribution.
#
# This software is distributed on an "AS IS" BASIS, WITHOUT ANY WARRANTY;
# without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
# PARTICULAR PURPOSE.


##############################################################################
# Functions
function usage {
  echo "usage: ${BASH_SOURCE[0]} <target architecture> [host os] [source dir]" >&2
  echo "  If no source directory is given, the current directory is assumed." >&2
}


##############################################################################
# Parameter checking
ARCH="${1}"
test -z "${ARCH}" && (usage ; exit 1)
HOST_OS="${2}"
test -z "${HOST_OS}" && HOST_OS="$(uname -o)"
SOURCE_DIR="${3}"
test -z "${SOURCE_DIR}" && SOURCE_DIR="$(pwd)"

##############################################################################
# Constants and defaults

# Directory of this script
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

##############################################################################
# Load functions
. "${SCRIPT_DIR}/scripts/chroot/chroot.sh"
. "${SCRIPT_DIR}/scripts/dependencies/dependencies.sh"

##############################################################################
# Main

set -e
set -x

# First, try to setup & enter chroot if necessary.
ret=0
chroot_try_enter "${BASH_SOURCE[0]}" "${ARCH}" "${HOST_OS}" "${SOURCE_DIR}" "wheezy" "$@" || ret="$?"

case "${ret}" in
  1)
    exit "$ret"
    ;;
  2)
    chroot_clean
    exit "$ret"
    ;;
  3)
    chroot_clean
    exit 0
    ;;
esac

# Install dependencies
dep_install "${SOURCE_DIR}"

# Build & Test
echo "======================================================================="
echo "Running build"
echo "Environment: $(uname -a)"

cd "${SOURCE_DIR}"
rm -rf build
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release -DMETA_USE_CXX11=${META_USE_CXX11} ..
make testsuite
./testsuite

# Cleanup
chroot_clean
