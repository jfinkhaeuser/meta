#!/bin/bash

set -e
set -x

function brew_install {
  pkg="$1"
  if brew list | grep -q "$pkg" ; then
    # Installed
    if brew outdated | grep -q "$pkg" ; then
      # Outdated
      brew upgrade "$@"
    else
      # Up-to-date
      true
    fi
  else
    # Not installed
    brew install "$@"
  fi
}

case "${TRAVIS_OS_NAME}" in
  linux)
    sudo apt-get install libcppunit-dev clang cmake
    ;;
  osx)
    brew_install cppunit
    brew_install cmake
#    brew install llvm --with-asan --with-clang --HEAD
    ;;
  *)
    echo "Unsupported os '${TRAVIS_OS_NAME}', aborting."
    exit 1
    ;;
esac
