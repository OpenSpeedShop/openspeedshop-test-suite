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

set(LULESH_SOURCES
	lulesh.cc
    )

message(STATUS "HAVE_OPENMPI=" ${HAVE_OPENMPI} )
message(STATUS "BUILD_COMPILER_NAME=" ${BUILD_COMPILER_NAME} )


# sequential build section ---------------------------------------------------------

add_executable(sequential-lulesh
    ${LULESH_SOURCES}
)

target_include_directories(sequential-lulesh PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(sequential-lulesh
    ${CMAKE_DL_LIBS}
)

set_target_properties(sequential-lulesh PROPERTIES OUTPUT_NAME "sequential-lulesh-${BUILD_COMPILER_NAME}")

install(TARGETS sequential-lulesh
    DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
)

# MPI build section ---------------------------------------------------------

if(HAVE_OPENMPI)

    add_executable(mpi-lulesh-openmpi
        ${LULESH_SOURCES}
    )

    target_include_directories(mpi-lulesh-openmpi PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${OpenMPI_INCLUDE_DIRS}
    )

    target_link_libraries(mpi-lulesh-openmpi
	${CMAKE_DL_LIBS}
	${OpenMPI_SHARED_LIBRARIES}
	m
    )

    set_target_properties(mpi-lulesh-openmpi PROPERTIES COMPILE_DEFINITIONS "${OpenMPI_DEFINES}")
    set_target_properties(mpi-lulesh-openmpi PROPERTIES OUTPUT_NAME "mpi-lulesh-openmpi-${BUILD_COMPILER_NAME}")

    install(TARGETS mpi-lulesh-openmpi
            DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
    )

endif()


if(HAVE_MVAPICH)

    add_executable(mpi-lulesh-mvapich
        ${LULESH_SOURCES}
    )

    target_include_directories(mpi-lulesh-mvapich PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${Mvapich_INCLUDE_DIRS}
    )

    target_link_libraries(mpi-lulesh-mvapich
	${CMAKE_DL_LIBS}
	${Mvapich_SHARED_LIBRARIES}
	m
    )

    set_target_properties(mpi-lulesh-mvapich PROPERTIES COMPILE_DEFINITIONS "${Mvapich_DEFINES}")
    set_target_properties(mpi-lulesh-mvapich PROPERTIES OUTPUT_NAME "mpi-lulesh-mvapich-${BUILD_COMPILER_NAME}")

    install(TARGETS mpi-lulesh-mvapich
            DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
    )

endif()

if(HAVE_MVAPICH2)

    add_executable(mpi-lulesh-mvapich2
        ${LULESH_SOURCES}
    )

    target_include_directories(mpi-lulesh-mvapich2 PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${Mvapich2_INCLUDE_DIRS}
    )

    target_link_libraries(mpi-lulesh-mvapich2
	${CMAKE_DL_LIBS}
	${Mvapich2_SHARED_LIBRARIES}
	m
    )

    set_target_properties(mpi-lulesh-mvapich2 PROPERTIES COMPILE_DEFINITIONS "${Mvapich2_DEFINES}")
    set_target_properties(mpi-lulesh-mvapich2 PROPERTIES OUTPUT_NAME "mpi-lulesh-mvapich2-${BUILD_COMPILER_NAME}")

    install(TARGETS mpi-lulesh-mvapich2
            DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
    )

endif()

if(HAVE_MPT)

    add_executable(mpi-lulesh-mpt
        ${LULESH_SOURCES}
    )

    target_include_directories(mpi-lulesh-mpt PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${Mpt_INCLUDE_DIRS}
    )

    target_link_libraries(mpi-lulesh-mpt
	${CMAKE_DL_LIBS}
	${Mpt_SHARED_LIBRARIES}
	m
    )

    set_target_properties(mpi-lulesh-mpt PROPERTIES COMPILE_DEFINITIONS "${Mpt_DEFINES}")
    set_target_properties(mpi-lulesh-mpt PROPERTIES OUTPUT_NAME "mpi-lulesh-mpt-${BUILD_COMPILER_NAME}")

    install(TARGETS mpi-lulesh-mpt
            DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
    )

endif()

if(HAVE_MPICH)

    add_executable(mpi-lulesh-mpich
        ${LULESH_SOURCES}
    )

    target_include_directories(mpi-lulesh-mpich PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${Mpich_INCLUDE_DIRS}
    )

    target_link_libraries(mpi-lulesh-mpich
	${CMAKE_DL_LIBS}
	${Mpich_SHARED_LIBRARIES}
	m
    )

    set_target_properties(mpi-lulesh-mpich PROPERTIES COMPILE_DEFINITIONS "${Mpich_DEFINES}")
    set_target_properties(mpi-lulesh-mpich PROPERTIES OUTPUT_NAME "mpi-lulesh-mpich-${BUILD_COMPILER_NAME}")

    install(TARGETS mpi-lulesh-mpich
            DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
    )

endif()

