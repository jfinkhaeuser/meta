#!/bin/bash

set -e
set -x

rm -rf build
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release -DMETA_USE_CXX11=${META_USE_CXX11} ..
make testsuite
./testsuite
