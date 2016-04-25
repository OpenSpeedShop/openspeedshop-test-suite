#! /bin/bash

rm -rf build_gnu_application_tests
mkdir -p build_gnu_application_tests
pushd build_gnu_application_tests

export OPENMPI_ROOT=/opt/openmpi-1.10.2
cmake .. \
       -DCMAKE_INSTALL_PREFIX=/opt/application_test_demos \
       -DCMAKE_BUILD_TYPE=None \
       -DCMAKE_CXX_FLAGS="-g -O2" \
       -DCMAKE_C_FLAGS="-g -O2" \
       -DOPENMPI_DIR=${OPENMPI_ROOT} \
       -DBUILD_COMPILER_NAME=gnu


make clean
make
make install

