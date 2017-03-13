################################################################################
# Copyright (c) 2014-2016 Krell Institute. All Rights Reserved.
#
# This program is free software; you can redistribute it and/or modify it under
# the terms of the GNU General Public License as published by the Free Software
# Foundation; either version 2 of the License, or (at your option) any later
# version.
#
# This program is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
# FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
# details.
#
# You should have received a copy of the GNU General Public License along with
# this program; if not, write to the Free Software Foundation, Inc., 59 Temple
# Place, Suite 330, Boston, MA  02111-1307  USA
################################################################################

cmake_minimum_required(VERSION 2.8.0)

project(openspeedshop_test_suite C CXX Fortran)
enable_language(Fortran)

set(OPENSPEEDSHOP_TEST_SUITE_VERSION 1.0.0)
set(OPENSPEEDSHOP_TEST_SUITE_VERSION_MAJOR 1)
set(OPENSPEEDSHOP_TEST_SUITE_VERSION_MINOR 0)
set(OPENSPEEDSHOP_TEST_SUITE_VERSION_PATCH 0)
set(OPENSPEEDSHOP_TEST_SUITE_VERSION_TWEAK 0)
add_definitions(-DVERSION="${OPENSPEEDSHOP_TEST_SUITE_VERSION}")

include(CMakeDetermineFortranCompiler)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake)

include(CheckIncludeFile)
include(CheckIncludeFileCXX)
include(CheckFunctionExists)
include(CheckLibraryExists)

check_include_file("stdint.h" STDINT_H_FOUND)
if (STDINT_H_FOUND)
    add_definitions(-DHAVE_STDINT_H=1)
endif()

check_include_file("stat.h" STAT_H_FOUND)
if (STAT_H_FOUND)
    add_definitions(-DHAVE_STAT_H=1)
endif()

check_include_file("sys/stat.h" SYS_STAT_H_FOUND)
if (SYS_STAT_H_FOUND)
    add_definitions(-DHAVE_SYS_STAT_H=1)
endif()

check_include_file(dirent.h HAVE_DIRENT_H)
check_include_file(dlfcn.h HAVE_DLFCN_H)
check_include_file(errno.h HAVE_ERRNO_H)
check_include_file(execinfo.h HAVE_EXECINFO_H)
check_include_file(fcntl.h HAVE_FCNTL_H)
check_include_file(inttypes.h HAVE_INTTYPES_H)
check_include_file(limits.h HAVE_LIMITS_H)
check_include_file(link.h HAVE_LINK_H)
check_include_file(malloc.h HAVE_MALLOC_H)
check_include_file(malloc/malloc.h HAVE_MALLOC_MALLOC_H)
check_function_exists(fork HAVE_FORK)

find_program(CMAKE_C_COMPILER NAMES $ENV{CC} gcc icc PATHS ENV PATH NO_DEFAULT_PATH)
find_program(CMAKE_CXX_COMPILER NAMES $ENV{CXX} g++ icpc PATHS ENV PATH NO_DEFAULT_PATH)
find_program(CMAKE_Fortran_COMPILER NAMES $ENV{CXX} gfortran ifort g77 f95 f90 PATHS ENV PATH NO_DEFAULT_PATH)


# FFLAGS depend on the compiler
get_filename_component (Fortran_COMPILER_NAME ${CMAKE_Fortran_COMPILER} NAME)

message(STATUS "Fortran_COMPILER_NAME=" ${Fortran_COMPILER_NAME} )

if (Fortran_COMPILER_NAME MATCHES "gfortran.*")
  # gfortran
  set (CMAKE_Fortran_FLAGS_RELEASE "-funroll-all-loops -fno-f2c -O3")
  set (CMAKE_Fortran_FLAGS_DEBUG   "-fno-f2c -O0 -g")
elseif (Fortran_COMPILER_NAME MATCHES "ifort.*")
  # ifort (untested)
  set (CMAKE_Fortran_FLAGS_RELEASE "-f77rtl -O3")
  set (CMAKE_Fortran_FLAGS_DEBUG   "-f77rtl -O0 -g")
elseif (Fortran_COMPILER_NAME MATCHES "g77")
  # g77
  set (CMAKE_Fortran_FLAGS_RELEASE "-funroll-all-loops -fno-f2c -O3 -m32")
  set (CMAKE_Fortran_FLAGS_DEBUG   "-fno-f2c -O0 -g -m32")
else (Fortran_COMPILER_NAME MATCHES "gfortran.*")
  message ("CMAKE_Fortran_COMPILER full path: " ${CMAKE_Fortran_COMPILER})
  message ("Fortran compiler: " ${Fortran_COMPILER_NAME})
  message ("No optimized Fortran compiler flags are known, we just try -O2...")
  set (CMAKE_Fortran_FLAGS_RELEASE "-O2")
  set (CMAKE_Fortran_FLAGS_DEBUG   "-O0 -g")
endif (Fortran_COMPILER_NAME MATCHES "gfortran.*")

#
#--------------------------------------------------------------------------------
# Do find package calls for the dependency packages that are commonly needed for 
# cbtf.
#--------------------------------------------------------------------------------
#
find_package(OpenMP)
find_package(OpenMPI)
find_package(Mpich)
find_package(Mpich2)
if (DEFINED MPT_BUILD)
	find_package(Mpt)
endif()
find_package(Mvapich2)
find_package(Mvapich)
find_package(CUDA)
find_package(CrayAlps)

if (CRAYALPS_FOUND)
  set(HAVE_CRAYALPS "true")
endif()

find_package(LibIomp)

if (LIBIOMP_FOUND)
  # This does not work for c++ libs. libiomp is compiled under the covers with c++.
  #CHECK_LIBRARY_EXISTS(${LibIomp_LIBRARY_SHARED} "ompt_init" "" HAVE_OMPT) 
  if (EXISTS "${LibIomp_INCLUDE_DIR}/ompt.h")
    set(OMPT_DEFINES "HAVE_OMPT=1")
    set(OMPT_FOUND "true")
  else()
    set(OMPT_FOUND "false")
  endif()
endif()
#
#--------------------------------------------------------------------------------
# Handle MPI settings here.  Set the appropriate flags for the rest of the tool
# to use when building MPI related libraries (collectors).
#--------------------------------------------------------------------------------
#
set(all_mpi_names "")

message(STATUS "OPENMPI_FOUND=" ${OPENMPI_FOUND} )


if (OPENMPI_FOUND)
  # This is needed by the mpi collectors
  set(HAVE_OPENMPI "true")
  set(HAVE_MPI "true")
  set(DEFAULT_MPI_IMPL_NAME "openmpi")
  set(DEFAULT_MPI_IMPL "OPENMPI")
  set(all_mpi_names ${all_mpi_names} " openmpi ")
endif()

if (MPICH2_FOUND)
  # This is needed by the mpi collectors
  set(HAVE_MPICH2 "true")
  set(HAVE_MPI "true")
  set(DEFAULT_MPI_IMPL_NAME "mpich2")
  set(DEFAULT_MPI_IMPL "MPICH2")
  set(all_mpi_names ${all_mpi_names} " mpich2 ")
endif()

if (MPICH_FOUND)
  # This is needed by the mpi collectors
  set(HAVE_MPICH "true")
  set(HAVE_MPI "true")
  set(DEFAULT_MPI_IMPL_NAME "mpich")
  set(DEFAULT_MPI_IMPL "MPICH")
  set(all_mpi_names ${all_mpi_names} " mpich ")
endif()

if (MPT_FOUND)
  # This is needed by the mpi collectors
  set(HAVE_MPT "true")
  set(HAVE_MPI "true")
  set(DEFAULT_MPI_IMPL_NAME "mpt")
  set(DEFAULT_MPI_IMPL "MPT")
  set(all_mpi_names ${all_mpi_names} " mpt ")
endif()

if (MVAPICH_FOUND)
  # This is needed by the mpi collectors
  set(HAVE_MVAPICH "true")
  set(HAVE_MPI "true")
  set(DEFAULT_MPI_IMPL_NAME "mvapich")
  set(DEFAULT_MPI_IMPL "MVAPICH")
  set(all_mpi_names ${all_mpi_names} " mvapich ")
endif()

if (MVAPICH2_FOUND)
  # This is needed by the mpi collectors
  set(HAVE_MVAPICH2 "true")
  set(HAVE_MPI "true")
  set(DEFAULT_MPI_IMPL_NAME "mvapich2")
  set(DEFAULT_MPI_IMPL "MVAPICH2")
  set(all_mpi_names ${all_mpi_names} " mvapich2 ")
endif()

set(ALL_MPI_IMPL_NAMES ${all_mpi_names})
#


set(OpenMP_RT_NAME "")
if (OpenMP_FLAG_DETECTED)
  if (${OpenMP_C_FLAGS} MATCHES "-fopenmp")
     set(OpenMP_RT_NAME "gomp")
  elseif (${OpenMP_C_FLAGS} MATCHES "-openmp")
     set(OpenMP_RT_NAME "iomp5")
  elseif (${OpenMP_C_FLAGS} MATCHES "-qopenmp")
     set(OpenMP_RT_NAME "iomp5")
  else()
     set(OpenMP_RT_NAME "gomp")
  endif()
endif()


add_subdirectory(src)

