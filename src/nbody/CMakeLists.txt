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

set(NBODY_SOURCES
	nbody-mpi.c
    )

message(STATUS "HAVE_OPENMPI=" ${HAVE_OPENMPI} )
message(STATUS "BUILD_COMPILER_NAME=" ${BUILD_COMPILER_NAME} )

if(HAVE_OPENMPI)

    add_executable(mpi-nbody-openmpi
        ${NBODY_SOURCES}
    )

    target_include_directories(mpi-nbody-openmpi PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${OpenMPI_INCLUDE_DIRS}
    )

    target_link_libraries(mpi-nbody-openmpi
	${CMAKE_DL_LIBS}
	${OpenMPI_SHARED_LIBRARIES}
	m
    )

    set_target_properties(mpi-nbody-openmpi PROPERTIES COMPILE_DEFINITIONS "${OpenMPI_DEFINES}")
    set_target_properties(mpi-nbody-openmpi PROPERTIES OUTPUT_NAME "mpi-nbody-openmpi-${BUILD_COMPILER_NAME}")

    install(TARGETS mpi-nbody-openmpi
            DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
    )
endif()
    
if(HAVE_MVAPICH)

    add_executable(mpi-nbody-mvapich
        ${NBODY_SOURCES}
    )

    target_include_directories(mpi-nbody-mvapich PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${Mvapich_INCLUDE_DIRS}
    )

    target_link_libraries(mpi-nbody-mvapich
	${CMAKE_DL_LIBS}
	${Mvapich_SHARED_LIBRARIES}
	m
    )

    set_target_properties(mpi-nbody-mvapich PROPERTIES COMPILE_DEFINITIONS "${Mvapich_DEFINES}")
    set_target_properties(mpi-nbody-mvapich PROPERTIES OUTPUT_NAME "mpi-nbody-mvapich-${BUILD_COMPILER_NAME}")

    install(TARGETS mpi-nbody-mvapich
            DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
    )

endif()
    
if(HAVE_MPT)

    add_executable(mpi-nbody-mpt
        ${NBODY_SOURCES}
    )

    target_include_directories(mpi-nbody-mpt PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${Mpt_INCLUDE_DIRS}
    )

    target_link_libraries(mpi-nbody-mpt
	${CMAKE_DL_LIBS}
	${Mpt_SHARED_LIBRARIES}
	m
    )

    set_target_properties(mpi-nbody-mpt PROPERTIES COMPILE_DEFINITIONS "${Mpt_DEFINES}")
    set_target_properties(mpi-nbody-mpt PROPERTIES OUTPUT_NAME "mpi-nbody-mpt-${BUILD_COMPILER_NAME}")

    install(TARGETS mpi-nbody-mpt
            DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
    )

endif()

if(HAVE_MPICH)

    add_executable(mpi-nbody-mpich
        ${NBODY_SOURCES}
    )

    target_include_directories(mpi-nbody-mpich PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${Mpich_INCLUDE_DIRS}
    )

    target_link_libraries(mpi-nbody-mpich
	${CMAKE_DL_LIBS}
	${Mpich_SHARED_LIBRARIES}
	m
    )

    set_target_properties(mpi-nbody-mpich PROPERTIES COMPILE_DEFINITIONS "${Mpich_DEFINES}")
    set_target_properties(mpi-nbody-mpich PROPERTIES OUTPUT_NAME "mpi-nbody-mpich-${BUILD_COMPILER_NAME}")

    install(TARGETS mpi-nbody-mpich
            DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
    )

endif()

