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

set(LULESH_SOURCES
        lulesh.cc  lulesh-comm.cc  lulesh.h  lulesh-init.cc  lulesh_tuple.h  lulesh-util.cc  lulesh-viz.cc
    )


message(STATUS "OpenMP_FLAG_DETECTED=" ${OpenMP_FLAG_DETECTED} )
message(STATUS "HAVE_OPENMPI=" ${HAVE_OPENMPI} )
message(STATUS "BUILD_COMPILER_NAME=" ${BUILD_COMPILER_NAME} )
message(STATUS "OpenMP_RT_NAME=" ${OpenMP_RT_NAME} )

if(OpenMP_FLAG_DETECTED)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS} -I.")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS} -I.")

    if(BUILD_COMPILER_NAME MATCHES intel)
       set(OPENMP_LIBS ${LibIomp_SHARED_LIBRARIES})
    elseif(BUILD_COMPILER_NAME MATCHES gnu)
       set(OPENMP_LIBS ${OpenMP_RT_NAME})
    endif()

endif()

if(HAVE_OPENMPI)

    # Hybrid OpenMP/MPI executable section -------------------------------------------

    add_executable(hybrid-lulesh-openmpi
        ${LULESH_SOURCES}
    )

    target_include_directories(hybrid-lulesh-openmpi PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${OpenMP_INCLUDE_DIRS}
	${OpenMPI_INCLUDE_DIRS}
    )

    target_link_libraries(hybrid-lulesh-openmpi
	${CMAKE_DL_LIBS}
	${OpenMPI_SHARED_LIBRARIES}
    ${OPENMP_LIBS}
	m
    )

    set_target_properties(hybrid-lulesh-openmpi PROPERTIES COMPILE_DEFINITIONS "${OpenMPI_DEFINES};USE_MPI=1")
    set_target_properties(hybrid-lulesh-openmpi PROPERTIES OUTPUT_NAME "hybrid-lulesh-openmpi-${BUILD_COMPILER_NAME}")

    install(TARGETS hybrid-lulesh-openmpi
            DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
    )


    # MPI executable section -------------------------------------------

    add_executable(mpi-lulesh2.0.3-openmpi
        ${LULESH_SOURCES}
    )

    target_include_directories(mpi-lulesh2.0.3-openmpi PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_BINARY_DIR}
        ${OpenMPI_INCLUDE_DIRS}
    )

    target_link_libraries(mpi-lulesh2.0.3-openmpi
        ${CMAKE_DL_LIBS}
        ${OpenMPI_SHARED_LIBRARIES}
        m
    )

    set_target_properties(mpi-lulesh2.0.3-openmpi PROPERTIES COMPILE_DEFINITIONS "${OpenMPI_DEFINES};USE_MPI=1")
    set_target_properties(mpi-lulesh2.0.3-openmpi PROPERTIES OUTPUT_NAME "mpi-lulesh-openmpi-${BUILD_COMPILER_NAME}")

    install(TARGETS mpi-lulesh2.0.3-openmpi
            DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
    )

endif()

if(HAVE_MPT)

    # Hybrid OpenMP/MPI executable section -------------------------------------------

    add_executable(hybrid-lulesh-mpt
        ${LULESH_SOURCES}
    )

    target_include_directories(hybrid-lulesh-mpt PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${OpenMP_INCLUDE_DIRS}
	${Mpt_INCLUDE_DIRS}
    )

    target_link_libraries(hybrid-lulesh-mpt
	${CMAKE_DL_LIBS}
	${Mpt_SHARED_LIBRARIES}
        ${OPENMP_LIBS}
	m
    )

    set_target_properties(hybrid-lulesh-mpt PROPERTIES COMPILE_DEFINITIONS "${Mpt_DEFINES};USE_MPI=1")
    set_target_properties(hybrid-lulesh-mpt PROPERTIES OUTPUT_NAME "hybrid-lulesh-mpt-${BUILD_COMPILER_NAME}")

    install(TARGETS hybrid-lulesh-mpt
            DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
    )


    # MPI executable section -------------------------------------------

    add_executable(mpi-lulesh2.0.3-mpt
        ${LULESH_SOURCES}
    )

    target_include_directories(mpi-lulesh2.0.3-mpt PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_BINARY_DIR}
        ${Mpt_INCLUDE_DIRS}
    )

    target_link_libraries(mpi-lulesh2.0.3-mpt
        ${CMAKE_DL_LIBS}
        ${Mpt_SHARED_LIBRARIES}
        m
    )

    set_target_properties(mpi-lulesh2.0.3-mpt PROPERTIES COMPILE_DEFINITIONS "${Mpt_DEFINES};USE_MPI=1")
    set_target_properties(mpi-lulesh2.0.3-mpt PROPERTIES OUTPUT_NAME "mpi-lulesh-mpt-${BUILD_COMPILER_NAME}")

    install(TARGETS mpi-lulesh2.0.3-mpt
            DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
    )

endif()

