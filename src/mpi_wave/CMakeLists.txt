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

set(MPI_WAVE_SOURCES
	mpi_wave.c
    )

message(STATUS "HAVE_OPENMPI=" ${HAVE_OPENMPI} )
message(STATUS "BUILD_COMPILER_NAME=" ${BUILD_COMPILER_NAME} )

#	${Mvapich2_INCLUDE_DIRS}

if(HAVE_OPENMPI)

    add_executable(mpi-mpi_wave-openmpi
        ${MPI_WAVE_SOURCES}
    )

    target_include_directories(mpi-mpi_wave-openmpi PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${OpenMPI_INCLUDE_DIRS}
    )

    target_link_libraries(mpi-mpi_wave-openmpi
	${CMAKE_DL_LIBS}
	${OpenMPI_SHARED_LIBRARIES}
	m
    )

    set_target_properties(mpi-mpi_wave-openmpi PROPERTIES COMPILE_DEFINITIONS "${OpenMPI_DEFINES}")
    set_target_properties(mpi-mpi_wave-openmpi PROPERTIES OUTPUT_NAME "mpi-mpi_wave-openmpi-${BUILD_COMPILER_NAME}")

    install(TARGETS mpi-mpi_wave-openmpi
            DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
    )
endif()
    
if(HAVE_MVAPICH)

    add_executable(mpi-mpi_wave-mvapich
        ${MPI_WAVE_SOURCES}
    )

    target_include_directories(mpi-mpi_wave-mvapich PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${Mvapich_INCLUDE_DIRS}
    )

    target_link_libraries(mpi-mpi_wave-mvapich
	${CMAKE_DL_LIBS}
	${Mvapich_SHARED_LIBRARIES}
	m
    )

    set_target_properties(mpi-mpi_wave-mvapich PROPERTIES COMPILE_DEFINITIONS "${Mvapich_DEFINES}")
    set_target_properties(mpi-mpi_wave-mvapich PROPERTIES OUTPUT_NAME "mpi-mpi_wave-mvapich-${BUILD_COMPILER_NAME}")

    install(TARGETS mpi-mpi_wave-mvapich
            DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
    )

endif()
    
    
if(HAVE_MVAPICH2)

    add_executable(mpi-mpi_wave-mvapich2
        ${MPI_WAVE_SOURCES}
    )

    target_include_directories(mpi-mpi_wave-mvapich2 PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${Mvapich2_INCLUDE_DIRS}
    )

    target_link_libraries(mpi-mpi_wave-mvapich2
	${CMAKE_DL_LIBS}
	${Mvapich2_SHARED_LIBRARIES}
	m
    )

    set_target_properties(mpi-mpi_wave-mvapich2 PROPERTIES COMPILE_DEFINITIONS "${Mvapich2_DEFINES}")
    set_target_properties(mpi-mpi_wave-mvapich2 PROPERTIES OUTPUT_NAME "mpi-mpi_wave-mvapich2-${BUILD_COMPILER_NAME}")

    install(TARGETS mpi-mpi_wave-mvapich2
            DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
    )

endif()
    
if(HAVE_MPT)

    add_executable(mpi-mpi_wave-mpt
        ${MPI_WAVE_SOURCES}
    )

    target_include_directories(mpi-mpi_wave-mpt PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${Mpt_INCLUDE_DIRS}
    )

    target_link_libraries(mpi-mpi_wave-mpt
	${CMAKE_DL_LIBS}
	${Mpt_SHARED_LIBRARIES}
	m
    )

    set_target_properties(mpi-mpi_wave-mpt PROPERTIES COMPILE_DEFINITIONS "${Mpt_DEFINES}")
    set_target_properties(mpi-mpi_wave-mpt PROPERTIES OUTPUT_NAME "mpi-mpi_wave-mpt-${BUILD_COMPILER_NAME}")

    install(TARGETS mpi-mpi_wave-mpt
            DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
    )

endif()
    
if(HAVE_MPICH)

    add_executable(mpi-mpi_wave-mpich
        ${MPI_WAVE_SOURCES}
    )

    target_include_directories(mpi-mpi_wave-mpich PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${Mpich_INCLUDE_DIRS}
    )

    target_link_libraries(mpi-mpi_wave-mpich
	${CMAKE_DL_LIBS}
	${Mpich_SHARED_LIBRARIES}
	m
    )

    set_target_properties(mpi-mpi_wave-mpich PROPERTIES COMPILE_DEFINITIONS "${Mpich_DEFINES}")
    set_target_properties(mpi-mpi_wave-mpich PROPERTIES OUTPUT_NAME "mpi-mpi_wave-mpich-${BUILD_COMPILER_NAME}")

    install(TARGETS mpi-mpi_wave-mpich
            DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
    )

endif()

