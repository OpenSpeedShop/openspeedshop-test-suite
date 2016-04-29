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

project (sweep3d C CXX Fortran)

set(SWEEP3d_SOURCES_SINGLE
	decomp.f  driver.f  flux_err.f  initialize.f  inner_auto.f  inner.f  octant.f  read_input.f  source.f  sweep.f
        single_stuff.f timers.c
    )

set(SWEEP3d_SOURCES_MPI
	decomp.f  driver.f  flux_err.f  initialize.f  inner_auto.f  inner.f  octant.f  read_input.f  source.f  sweep.f
        msg_stuff.f timers.c
    )

message(STATUS "HAVE_OPENMPI=" ${HAVE_OPENMPI} )
message(STATUS "BUILD_COMPILER_NAME=" ${BUILD_COMPILER_NAME} )

add_custom_command(
    OUTPUT
        ${CMAKE_CURRENT_BINARY_DIR}/single_stuff.f
    COMMAND cpp -P
        ${CMAKE_CURRENT_SOURCE_DIR}/msg_stuff.cpp ${CMAKE_CURRENT_BINARY_DIR}/single_stuff.f
    DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}//msg_stuff.cpp
)
add_executable(sequential-sweep3d
    ${SWEEP3d_SOURCES_SINGLE}
)

target_include_directories(sequential-sweep3d PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(sequential-sweep3d
    ${CMAKE_DL_LIBS}
    m
)

set_target_properties(sequential-sweep3d PROPERTIES OUTPUT_NAME "sequential-sweep3d-${BUILD_COMPILER_NAME}")

install(TARGETS sequential-sweep3d
    DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
)

if(HAVE_OPENMPI)

    add_custom_command(
        OUTPUT
            ${CMAKE_CURRENT_BINARY_DIR}/msg_stuff.f
        COMMAND cpp -P -DMPI
            ${CMAKE_CURRENT_SOURCE_DIR}/msg_stuff.cpp ${CMAKE_CURRENT_BINARY_DIR}/msg_stuff.f
        DEPENDS
            ${CMAKE_CURRENT_SOURCE_DIR}//msg_stuff.cpp
    )

    add_executable(mpi-sweep3d-openmpi
        ${SWEEP3d_SOURCES_MPI}
    )

    target_include_directories(mpi-sweep3d-openmpi PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${OpenMPI_INCLUDE_DIRS}
    )

    target_link_libraries(mpi-sweep3d-openmpi
	${CMAKE_DL_LIBS}
	${OpenMPI_SHARED_LIBRARIES}
	m
    )

    set_target_properties(mpi-sweep3d-openmpi PROPERTIES COMPILE_DEFINITIONS "${OpenMPI_DEFINES};MPI")
    set_target_properties(mpi-sweep3d-openmpi PROPERTIES OUTPUT_NAME "mpi-sweep3d-openmpi-${BUILD_COMPILER_NAME}")

    install(TARGETS mpi-sweep3d-openmpi
            DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
    )
endif()

if(HAVE_MPT)

    add_custom_command(
        OUTPUT
            ${CMAKE_CURRENT_BINARY_DIR}/msg_stuff.f
        COMMAND cpp -P -DMPI
            ${CMAKE_CURRENT_SOURCE_DIR}/msg_stuff.cpp ${CMAKE_CURRENT_BINARY_DIR}/msg_stuff.f
        DEPENDS
            ${CMAKE_CURRENT_SOURCE_DIR}//msg_stuff.cpp
    )

    add_executable(mpi-sweep3d-mpt
        ${SWEEP3d_SOURCES_MPI}
    )

    target_include_directories(mpi-sweep3d-mpt PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${Mpt_INCLUDE_DIRS}
    )

    target_link_libraries(mpi-sweep3d-mpt
	${CMAKE_DL_LIBS}
	${Mpt_SHARED_LIBRARIES}
	m
    )

    set_target_properties(mpi-sweep3d-mpt PROPERTIES COMPILE_DEFINITIONS "${Mpt_DEFINES};MPI")
    set_target_properties(mpi-sweep3d-mpt PROPERTIES OUTPUT_NAME "mpi-sweep3d-mpt-${BUILD_COMPILER_NAME}")

    install(TARGETS mpi-sweep3d-mpt
            DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
    )
endif()

