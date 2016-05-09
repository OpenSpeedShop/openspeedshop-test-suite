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

set(IOR_POSIX_SOURCES
	aiori.h  defaults.h  IOR-aiori.h  iordef.h  IOR.h aiori-POSIX.c  IOR.c  parse_options.c  utilities.c aiori-noMPIIO.c aiori-noHDF5.c aiori-noNCMPI.c
    )

message(STATUS "HAVE_OPENMPI=" ${HAVE_OPENMPI} )
message(STATUS "BUILD_COMPILER_NAME=" ${BUILD_COMPILER_NAME} )


if(HAVE_OPENMPI)

    add_executable(mpi-ior-openmpi
        ${IOR_POSIX_SOURCES}
    )

    target_include_directories(mpi-ior-openmpi PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${OpenMPI_INCLUDE_DIRS}
    )

    target_link_libraries(mpi-ior-openmpi
	${CMAKE_DL_LIBS}
	${OpenMPI_SHARED_LIBRARIES}
	m
    )

    set_target_properties(mpi-ior-openmpi PROPERTIES COMPILE_DEFINITIONS "${OpenMPI_DEFINES};_LARGEFILE64_SOURCE ;_FILE_OFFSET_BITS=64")
    set_target_properties(mpi-ior-openmpi PROPERTIES OUTPUT_NAME "mpi-ior-openmpi-${BUILD_COMPILER_NAME}")

    install(TARGETS mpi-ior-openmpi
            DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
    )
endif()
    
if(HAVE_MVAPICH)

    add_executable(mpi-ior-mvapich
        ${IOR_POSIX_SOURCES}
    )

    target_include_directories(mpi-ior-mvapich PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${Mvapich_INCLUDE_DIRS}
    )

    target_link_libraries(mpi-ior-mvapich
	${CMAKE_DL_LIBS}
	${Mvapich_SHARED_LIBRARIES}
	m
    )

    set_target_properties(mpi-ior-mvapich PROPERTIES COMPILE_DEFINITIONS "${Mvapich_DEFINES}")
    set_target_properties(mpi-ior-mvapich PROPERTIES OUTPUT_NAME "mpi-ior-mvapich-${BUILD_COMPILER_NAME}")

    install(TARGETS mpi-ior-mvapich
            DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
    )

endif()
    
if(HAVE_MPT)

    add_executable(mpi-ior-mpt
        ${IOR_POSIX_SOURCES}
    )

    target_include_directories(mpi-ior-mpt PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${Mpt_INCLUDE_DIRS}
    )

    target_link_libraries(mpi-ior-mpt
	${CMAKE_DL_LIBS}
	${Mpt_SHARED_LIBRARIES}
	m
    )

    set_target_properties(mpi-ior-mpt PROPERTIES COMPILE_DEFINITIONS "${Mpt_DEFINES}")
    set_target_properties(mpi-ior-mpt PROPERTIES OUTPUT_NAME "mpi-ior-mpt-${BUILD_COMPILER_NAME}")

    install(TARGETS mpi-ior-mpt
            DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
    )

endif()

if(HAVE_MPICH)

    add_executable(mpi-ior-mpich
        ${IOR_POSIX_SOURCES}
    )

    target_include_directories(mpi-ior-mpich PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${Mpich_INCLUDE_DIRS}
    )

    target_link_libraries(mpi-ior-mpich
	${CMAKE_DL_LIBS}
	${Mpich_SHARED_LIBRARIES}
	m
    )

    set_target_properties(mpi-ior-mpich PROPERTIES COMPILE_DEFINITIONS "${Mpich_DEFINES}")
    set_target_properties(mpi-ior-mpich PROPERTIES OUTPUT_NAME "mpi-ior-mpich-${BUILD_COMPILER_NAME}")

    install(TARGETS mpi-ior-mpich
            DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
    )

endif()

