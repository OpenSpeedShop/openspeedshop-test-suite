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

set(STREAM_SOURCES
	stream.c
    )

message(STATUS "HAVE_MPT=" ${HAVE_MPT} )
message(STATUS "BUILD_COMPILER_NAME=" ${BUILD_COMPILER_NAME} )
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


#	${Mvapich2_INCLUDE_DIRS}
if(HAVE_OPENMPI)

    add_executable(mpi-stream-openmpi
        ${STREAM_SOURCES}
    )

    target_include_directories(mpi-stream-openmpi PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${OpenMPI_INCLUDE_DIRS}
    )

    target_link_libraries(mpi-stream-openmpi
	${CMAKE_DL_LIBS}
	${OpenMPI_SHARED_LIBRARIES}
	m
    )

    set_target_properties(mpi-stream-openmpi PROPERTIES COMPILE_DEFINITIONS "${OpenMPI_DEFINES}")
    set_target_properties(mpi-stream-openmpi PROPERTIES OUTPUT_NAME "mpi-stream-openmpi-${BUILD_COMPILER_NAME}")

    install(TARGETS mpi-stream-openmpi
            DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
    )

    # Hybrid OpenMP/MPI executable section -------------------------------------------

    add_executable(hybrid-stream-openmpi
        ${STREAM_SOURCES}
    )

    target_include_directories(hybrid-stream-openmpi PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_BINARY_DIR}
        ${OpenMP_INCLUDE_DIRS}
        ${OpenMPI_INCLUDE_DIRS}
    )

    target_link_libraries(hybrid-stream-openmpi
        ${CMAKE_DL_LIBS}
        ${OpenMPI_SHARED_LIBRARIES}
        ${OPENMP_LIBS}
        m
    )

    set_target_properties(hybrid-stream-openmpi PROPERTIES COMPILE_DEFINITIONS "${OpenMPI_DEFINES};USE_MPI=1")
    set_target_properties(hybrid-stream-openmpi PROPERTIES OUTPUT_NAME "hybrid-stream-openmpi-${BUILD_COMPILER_NAME}")

    install(TARGETS hybrid-stream-openmpi
            DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
    )


endif()
    
if(HAVE_MVAPICH2)

    add_executable(mpi-stream-mvapich2
        ${STREAM_SOURCES}
    )

    target_include_directories(mpi-stream-mvapich2 PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${Mvapich2_INCLUDE_DIRS}
    )

    target_link_libraries(mpi-stream-mvapich2
	${CMAKE_DL_LIBS}
	${Mvapich2_SHARED_LIBRARIES}
	m
    )

    set_target_properties(mpi-stream-mvapich2 PROPERTIES COMPILE_DEFINITIONS "${Mvapich2_DEFINES}")
    set_target_properties(mpi-stream-mvapich2 PROPERTIES OUTPUT_NAME "mpi-stream-mvapich2-${BUILD_COMPILER_NAME}")

    install(TARGETS mpi-stream-mvapich2
            DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
    )

    # Hybrid Mvapich2/MPI executable section -------------------------------------------

    add_executable(hybrid-stream-mvapich2
        ${STREAM_SOURCES}
    )

    target_include_directories(hybrid-stream-mvapich2 PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_BINARY_DIR}
        ${OpenMP_INCLUDE_DIRS}
        ${Mvapich2_INCLUDE_DIRS}
    )

    target_link_libraries(hybrid-stream-mvapich2
        ${CMAKE_DL_LIBS}
        ${Mvapich2_SHARED_LIBRARIES}
        m
    )

    set_target_properties(hybrid-stream-mvapich2 PROPERTIES COMPILE_DEFINITIONS "${Mvapich2_DEFINES};USE_MPI=1")
    set_target_properties(hybrid-stream-mvapich2 PROPERTIES OUTPUT_NAME "hybrid-stream-openmpi-${BUILD_COMPILER_NAME}")

    install(TARGETS hybrid-stream-mvapich2
            DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
    )

endif()
    
if(HAVE_MPT)

    add_executable(mpi-stream-mpt
        ${STREAM_SOURCES}
    )

    target_include_directories(mpi-stream-mpt PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${Mpt_INCLUDE_DIRS}
    )

    target_link_libraries(mpi-stream-mpt
	${CMAKE_DL_LIBS}
	${Mpt_SHARED_LIBRARIES}
	m
    )

    set_target_properties(mpi-stream-mpt PROPERTIES COMPILE_DEFINITIONS "${Mpt_DEFINES}")
    set_target_properties(mpi-stream-mpt PROPERTIES OUTPUT_NAME "mpi-stream-mpt-${BUILD_COMPILER_NAME}")

    install(TARGETS mpi-stream-mpt
            DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
    )

    # Hybrid Mpt/MPI executable section -------------------------------------------

    add_executable(hybrid-stream-mpt
        ${STREAM_SOURCES}
    )

    target_include_directories(hybrid-stream-mpt PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_BINARY_DIR}
        ${OpenMP_INCLUDE_DIRS}
        ${Mpt_INCLUDE_DIRS}
    )

    target_link_libraries(hybrid-stream-mpt
        ${CMAKE_DL_LIBS}
        ${Mpt_SHARED_LIBRARIES}
        m
    )

    set_target_properties(hybrid-stream-mpt PROPERTIES COMPILE_DEFINITIONS "${Mpt_DEFINES};USE_MPI=1")
    set_target_properties(hybrid-stream-mpt PROPERTIES OUTPUT_NAME "hybrid-stream-openmpi-${BUILD_COMPILER_NAME}")

    install(TARGETS hybrid-stream-mpt
            DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
    )


endif()
    
