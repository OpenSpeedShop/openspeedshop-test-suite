#! /bin/bash

rm -rf build_intel_application_tests
mkdir -p build_intel_application_tests
pushd build_intel_application_tests

export OPENMPI_ROOT=/opt/openmpi-1.10.2
export LIBIOMP_ROOT=/opt/ompt_v2.2.2
cmake .. \
       -DCMAKE_INSTALL_PREFIX=/opt/application_test_demos \
       -DCMAKE_CXX_COMPILER=icpc \
       -DCMAKE_C_COMPILER=icc \
       -DCMAKE_BUILD_TYPE=None \
       -DCMAKE_CXX_FLAGS="-g -O2" \
       -DCMAKE_C_FLAGS="-g -O2" \
       -DOPENMPI_DIR=${OPENMPI_ROOT} \
       -DLIBIOMP_DIR=${LIBIOMP_ROOT} \
       -DBUILD_COMPILER_NAME=intel


make clean
make
make install

