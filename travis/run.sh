#!/bin/bash
# Based on instructions from https://www.tomaz.me/2013/12/02/running-travis-ci-tests-on-arm.html

set -e
set -x

##############################################################################
# Parameter checking
test -z "${HOST_OS}" && exit 1
test -z "${ARCH}" && exit 2
test -z "${SOURCE_DIR}" && SOURCE_DIR="$(pwd)"

##############################################################################
# Constants and defaults

# Directory of this script
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

##############################################################################
# Load functions
. "${SCRIPT_DIR}/chroot.sh"

##############################################################################
# Main

# First, try to setup & enter chroot if necessary.
chroot_try_enter

#
#
#
#if [ -e "/.chroot_is_done" ]; then
#  # We are inside chroot
#  echo "Running inside chrooted environment"
#
#  . ./envvars.sh
#else
#  if [ "${ARCH}" != "native" ]; then
#    if [ "${TRAVIS_OS_NAME}" = "osx" ]; then
#      echo "Won't emulate on OS X, all done!"
#      exit 0
#    fi
#
#    # Emulated test run, need to set up chrooted environment first
#    echo "Setting up chrooted environment"
#    setup_chroot
#  fi
#fi
#
#echo "Running tests"
#echo "Environment: $(uname -a)"
#
## Build
#cd "${TRAVIS_BUILD_DIR}"
#./travis/before_install.sh
#./travis/install.sh
#./travis/build.sh
#
## Cleanup
#clean_chroot
