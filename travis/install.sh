#!/bin/bash

case "${TRAVIS_OS_NAME}" in
  linux)
    sudo apt-get install libcppunit-dev clang cmake
    ;;
  osx)
    brew install cppunit clang cmake
    ;;
  *)
    echo "Unsupported os '${TRAVIS_OS_NAME}', aborting."
    exit 1
    ;;
esac
