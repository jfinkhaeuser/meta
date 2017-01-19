#!/bin/bash
# Based on instructions from https://www.tomaz.me/2013/12/02/running-travis-ci-tests-on-arm.html

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
. "${SCRIPT_DIR}/travis/chroot.sh"

##############################################################################
# Main

set -e
# set -x

# First, try to setup & enter chroot if necessary.
chroot_try_enter "${BASH_SOURCE[0]}" "${ARCH}" "${HOST_OS}" "${SOURCE_DIR}" "$@"

case "$?" in
  0)
    # We're in target architecture, proceed
    ;;
  1)
    # Unsupported host OS, continue with host build
    ;;
  2)
    # Finished inside chroot, we're done.
    exit 0
    ;;
esac

# Build
echo "Running build"
echo "Environment: $(uname -a)"

cd "${SOURCE_DIR}"
./travis/before_install.sh
./travis/install.sh
./travis/build.sh

# Cleanup
chroot_clean
