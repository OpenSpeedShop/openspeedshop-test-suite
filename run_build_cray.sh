#! /bin/bash

rm -rf build_cray_application_tests
mkdir -p build_cray_application_tests
pushd build_cray_application_tests

export BASE_IDIR=/p/home/galarowi/OSS
export CMAKE_IDIR=${BASE_IDIR}/cmake-3.2.2
export PATH=${CMAKE_IDIR}/bin:$PATH
echo $PATH


export MPICH2_ROOT=/opt/cray/mpt/6.3.1/gni/mpich2-cray/81
export LIBIOMP_ROOT=/p/home/galarowi/OSS/krellroot_v2.2.3

cmake .. \
       -DCMAKE_INSTALL_PREFIX=/p/home/galarowi/application_test_demos \
       -DCMAKE_BUILD_TYPE=None \
       -DCMAKE_CXX_FLAGS="-g -O2" \
       -DCMAKE_C_FLAGS="-g -O2" \
       -DCMAKE_C_COMPILER="cc" \
       -DCMAKE_CXX_COMPILER="CC" \
       -DCMAKE_Fortran_COMPILER="ftn" \
       -DMPICH2_DIR=${MPICH2_ROOT} \
       -DLIBIOMP_DIR=${LIBIOMP_ROOT} \
       -DBUILD_COMPILER_NAME=cray


make clean
make
make install

