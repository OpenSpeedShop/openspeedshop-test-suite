#! /bin/bash
pushd ..
rm -rf build_intel_application_tests
mkdir -p build_intel_application_tests
pushd build_intel_application_tests

export MPT_DIR=
export LIBIOMP_ROOT=/nobackupnfs2/jgalarow/krellroot_v2.2.6


cmake .. \
       -DCMAKE_INSTALL_PREFIX=..\
       -DCMAKE_CXX_COMPILER=icpc \
       -DCMAKE_C_COMPILER=icc \
       -DCMAKE_BUILD_TYPE=None \
       -DCMAKE_CXX_FLAGS="-g -O2" \
       -DCMAKE_C_FLAGS="-g -O2" \
       -DMPT_DIR=${OPENMPI_ROOT} \
       -DLIBIOMP_DIR=${LIBIOMP_ROOT} \
       -DBUILD_COMPILER_NAME=intel


make clean
make
make install
rm -rf build_intel_application_tests
