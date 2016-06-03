################################################################################
# Copyright (c) 2016 Krell Institute. All Rights Reserved.
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

project (lassen C CXX)

#set(HAVE_SILO 0)

set(LASSEN_SOURCES_SINGLE
    lassenSerial.cxx
)

set(LIBLASSEN_SOURCES_COMMON
    Simulation.cxx Lassen.cxx Source.cxx Output.cxx LassenUtil.cxx Input.cxx
)

set(LASSEN_SOURCES_MPI
    SimulationParallel.cxx SimulationMPI.cxx
)

message(STATUS "HAVE_OPENMPI=" ${HAVE_OPENMPI} )
message(STATUS "BUILD_COMPILER_NAME=" ${BUILD_COMPILER_NAME} )

add_library(front STATIC
    ${LIBLASSEN_SOURCES_COMMON}
)

target_include_directories(front PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(front
    ${CMAKE_DL_LIBS}
)

set_target_properties(front PROPERTIES OUTPUT_NAME "front-${BUILD_COMPILER_NAME}")
#set_target_properties(front PROPERTIES COMPILE_DEFINITIONS "HAVE_SILO=${HAVE_SILO}")


install(TARGETS front ARCHIVE
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
)

add_executable(sequential-lassen
    ${LASSEN_SOURCES_SINGLE}
)

target_include_directories(sequential-lassen PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(sequential-lassen
    ${CMAKE_DL_LIBS}
    ${CMAKE_CURRENT_BINARY_DIR}/libfront-${BUILD_COMPILER_NAME}.a
    m
)

set_target_properties(sequential-lassen PROPERTIES OUTPUT_NAME "sequential-lassen-${BUILD_COMPILER_NAME}")
#set_target_properties(sequential-lassen PROPERTIES COMPILE_DEFINITIONS "HAVE_SILO=${HAVE_SILO}")

install(TARGETS sequential-lassen
    DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
)

# Disable MPI for now - can't figure out a linker issue with no main found
if (0)
if(HAVE_OPENMPI)

    add_executable(mpi-lassen-openmpi
        ${LASSEN_SOURCES_MPI}
    )

    target_include_directories(mpi-lassen-openmpi PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${OpenMPI_INCLUDE_DIRS}
    )

    target_link_libraries(mpi-lassen-openmpi
	${CMAKE_DL_LIBS}
	${OpenMPI_SHARED_LIBRARIES}
        ${CMAKE_CURRENT_BINARY_DIR}/libfront-${BUILD_COMPILER_NAME}.a
	m
    )

    #set_target_properties(mpi-lassen-openmpi PROPERTIES COMPILE_DEFINITIONS "${OpenMPI_DEFINES};HAVE_SILO=${HAVE_SILO}")
    set_target_properties(mpi-lassen-openmpi PROPERTIES COMPILE_DEFINITIONS "${OpenMPI_DEFINES}")
    set_target_properties(mpi-lassen-openmpi PROPERTIES OUTPUT_NAME "mpi-lassen-openmpi-${BUILD_COMPILER_NAME}")

    install(TARGETS mpi-lassen-openmpi
            DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
    )
endif()

if(HAVE_MPT)

    add_executable(mpi-lassen-mpt
        ${LASSEN_SOURCES_MPI}
    )

    target_include_directories(mpi-lassen-mpt PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${Mpt_INCLUDE_DIRS}
    )

    target_link_libraries(mpi-lassen-mpt
	${CMAKE_DL_LIBS}
	${Mpt_SHARED_LIBRARIES}
        ${CMAKE_CURRENT_BINARY_DIR}/libfront-${BUILD_COMPILER_NAME}.a
	m
    )

    #set_target_properties(mpi-lassen-mpt PROPERTIES COMPILE_DEFINITIONS "${Mpt_DEFINES};HAVE_SILO=${HAVE_SILO}")
    set_target_properties(mpi-lassen-mpt PROPERTIES COMPILE_DEFINITIONS "${Mpt_DEFINES}")
    set_target_properties(mpi-lassen-mpt PROPERTIES OUTPUT_NAME "mpi-lassen-mpt-${BUILD_COMPILER_NAME}")

    install(TARGETS mpi-lassen-mpt
            DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
    )
endif()
endif()

