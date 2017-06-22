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

    add_executable(mpi-nbodyPcontrol-openmpi
        ${NBODY_SOURCES}
    )

    target_include_directories(mpi-nbodyPcontrol-openmpi PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${OpenMPI_INCLUDE_DIRS}
    )

    target_link_libraries(mpi-nbodyPcontrol-openmpi
	${CMAKE_DL_LIBS}
	${OpenMPI_SHARED_LIBRARIES}
	m
    )

    set_target_properties(mpi-nbodyPcontrol-openmpi PROPERTIES COMPILE_DEFINITIONS "${OpenMPI_DEFINES}")
    set_target_properties(mpi-nbodyPcontrol-openmpi PROPERTIES OUTPUT_NAME "mpi-nbodyPcontrol-openmpi-${BUILD_COMPILER_NAME}")

    install(TARGETS mpi-nbodyPcontrol-openmpi
            DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
    )
endif()
    
if(HAVE_MVAPICH)

    add_executable(mpi-nbodyPcontrol-mvapich
        ${NBODY_SOURCES}
    )

    target_include_directories(mpi-nbodyPcontrol-mvapich PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${Mvapich_INCLUDE_DIRS}
    )

    target_link_libraries(mpi-nbodyPcontrol-mvapich
	${CMAKE_DL_LIBS}
	${Mvapich_SHARED_LIBRARIES}
	m
    )

    set_target_properties(mpi-nbodyPcontrol-mvapich PROPERTIES COMPILE_DEFINITIONS "${Mvapich_DEFINES}")
    set_target_properties(mpi-nbodyPcontrol-mvapich PROPERTIES OUTPUT_NAME "mpi-nbodyPcontrol-mvapich-${BUILD_COMPILER_NAME}")

    install(TARGETS mpi-nbodyPcontrol-mvapich
            DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
    )

endif()
    
if(HAVE_MPT)

    add_executable(mpi-nbodyPcontrol-mpt
        ${NBODY_SOURCES}
    )

    target_include_directories(mpi-nbodyPcontrol-mpt PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${Mpt_INCLUDE_DIRS}
    )

    target_link_libraries(mpi-nbodyPcontrol-mpt
	${CMAKE_DL_LIBS}
	${Mpt_SHARED_LIBRARIES}
	m
    )

    set_target_properties(mpi-nbodyPcontrol-mpt PROPERTIES COMPILE_DEFINITIONS "${Mpt_DEFINES}")
    set_target_properties(mpi-nbodyPcontrol-mpt PROPERTIES OUTPUT_NAME "mpi-nbodyPcontrol-mpt-${BUILD_COMPILER_NAME}")

    install(TARGETS mpi-nbodyPcontrol-mpt
            DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
    )

endif()

if(HAVE_MPICH)

    add_executable(mpi-nbodyPcontrol-mpich
        ${NBODY_SOURCES}
    )

    target_include_directories(mpi-nbodyPcontrol-mpich PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${Mpich_INCLUDE_DIRS}
    )

    target_link_libraries(mpi-nbodyPcontrol-mpich
	${CMAKE_DL_LIBS}
	${Mpich_SHARED_LIBRARIES}
	m
    )

    set_target_properties(mpi-nbodyPcontrol-mpich PROPERTIES COMPILE_DEFINITIONS "${Mpich_DEFINES}")
    set_target_properties(mpi-nbodyPcontrol-mpich PROPERTIES OUTPUT_NAME "mpi-nbodyPcontrol-mpich-${BUILD_COMPILER_NAME}")

    install(TARGETS mpi-nbodyPcontrol-mpich
            DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
    )

endif()

