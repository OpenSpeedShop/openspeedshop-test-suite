#! /bin/bash
pushd ..
rm -rf build_pgi_application_tests
mkdir -p build_pgi_application_tests
pushd build_pgi_application_tests

export OPENMPI_ROOT=/opt/openmpi-1.10.2
export BASE_DIR=../bin

cmake .. \
       -DCMAKE_INSTALL_PREFIX=${BASE_DIR} \
       -DCMAKE_BUILD_TYPE=None \
       -DCMAKE_CXX_FLAGS="-g -O2" \
       -DCMAKE_C_FLAGS="-g -O2" \
       -DOPENMPI_DIR=${OPENMPI_ROOT} \
       -DBUILD_COMPILER_NAME=pgi


make clean
make
make install
rm -rf build_pgi_application_tests

