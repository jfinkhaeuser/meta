#!/bin/bash

set -e
set -x

case "${TRAVIS_OS_NAME}" in
  linux)
    sudo apt-get update -qq
    ;;
  osx)
    brew update
    ;;
  *)
    echo "Unsupported os '${TRAVIS_OS_NAME}', aborting."
    exit 1
    ;;
esac
