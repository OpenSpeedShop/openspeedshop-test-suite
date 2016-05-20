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

set(AMG2013_SOURCES
	amg2013.c
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


if(HAVE_OPENMPI)

    add_executable(mpi-amg2013-openmpi
        ${AMG2013_SOURCES}
    )

    target_include_directories(mpi-amg2013-openmpi PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/..
	${CMAKE_CURRENT_SOURCE_DIR}/../utilities
	${CMAKE_CURRENT_SOURCE_DIR}/../struct_mv
	${CMAKE_CURRENT_SOURCE_DIR}/../sstruct_mv
	${CMAKE_CURRENT_SOURCE_DIR}/../IJ_mv
	${CMAKE_CURRENT_SOURCE_DIR}/../seq_mv
	${CMAKE_CURRENT_SOURCE_DIR}/../parcsr_mv
	${CMAKE_CURRENT_SOURCE_DIR}/../parcsr_ls
	${CMAKE_CURRENT_SOURCE_DIR}/../krylov
	${CMAKE_CURRENT_BINARY_DIR}
	${OpenMPI_INCLUDE_DIRS}
    )

    target_link_libraries(mpi-amg2013-openmpi
	${CMAKE_DL_LIBS}
	${OpenMPI_SHARED_LIBRARIES}
        ${CMAKE_CURRENT_BINARY_DIR}/../parcsr_ls/libparcsr_ls_openmpi_mpi.a
        ${CMAKE_CURRENT_BINARY_DIR}/../parcsr_mv/libparcsr_mv_openmpi_mpi.a
        ${CMAKE_CURRENT_BINARY_DIR}/../IJ_mv/libIJ_mv_openmpi_mpi.a
        ${CMAKE_CURRENT_BINARY_DIR}/../seq_mv/libseq_mv_openmpi_mpi.a
        ${CMAKE_CURRENT_BINARY_DIR}/../struct_mv/libHYPRE_struct_mv_openmpi_mpi.a
        ${CMAKE_CURRENT_BINARY_DIR}/../sstruct_mv/libsstruct_mv_openmpi_mpi.a
        ${CMAKE_CURRENT_BINARY_DIR}/../krylov/libkrylov_openmpi_mpi.a
        ${CMAKE_CURRENT_BINARY_DIR}/../utilities/libHYPRE_utilities_openmpi_mpi.a
	m
    )

    set_target_properties(mpi-amg2013-openmpi PROPERTIES COMPILE_DEFINITIONS "${OpenMPI_DEFINES};TIMER_USE_MPI=1;HYPRE_LONG_LONG;HYPRE_NO_GLOBAL_PARTITION;HYPRE_TIMING")
    set_target_properties(mpi-amg2013-openmpi PROPERTIES OUTPUT_NAME "mpi-amg2013-openmpi-${BUILD_COMPILER_NAME}")

    install(TARGETS mpi-amg2013-openmpi
            DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
    )

    # Hybrid OpenMP/MPI executable section -------------------------------------------

    if(OpenMP_FLAG_DETECTED)
        add_executable(hybrid-amg2013-openmpi
            ${AMG2013_SOURCES}
        )

        target_include_directories(hybrid-amg2013-openmpi PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}
	    ${CMAKE_CURRENT_SOURCE_DIR}/..
	    ${CMAKE_CURRENT_SOURCE_DIR}/../utilities
    	    ${CMAKE_CURRENT_SOURCE_DIR}/../struct_mv
	    ${CMAKE_CURRENT_SOURCE_DIR}/../sstruct_mv
	    ${CMAKE_CURRENT_SOURCE_DIR}/../IJ_mv
	    ${CMAKE_CURRENT_SOURCE_DIR}/../seq_mv
	    ${CMAKE_CURRENT_SOURCE_DIR}/../parcsr_mv
	    ${CMAKE_CURRENT_SOURCE_DIR}/../parcsr_ls
	    ${CMAKE_CURRENT_SOURCE_DIR}/../krylov
            ${CMAKE_CURRENT_BINARY_DIR}
            ${OpenMP_INCLUDE_DIRS}
            ${OpenMPI_INCLUDE_DIRS}
        )

        target_link_libraries(hybrid-amg2013-openmpi
            ${CMAKE_DL_LIBS}
            ${CMAKE_CURRENT_BINARY_DIR}/../parcsr_ls/libparcsr_ls_openmpi_hybrid.a
            ${CMAKE_CURRENT_BINARY_DIR}/../parcsr_mv/libparcsr_mv_openmpi_hybrid.a
            ${CMAKE_CURRENT_BINARY_DIR}/../IJ_mv/libIJ_mv_openmpi_hybrid.a
            ${CMAKE_CURRENT_BINARY_DIR}/../seq_mv/libseq_mv_openmpi_hybrid.a
            ${CMAKE_CURRENT_BINARY_DIR}/../struct_mv/libHYPRE_struct_mv_openmpi_hybrid.a
            ${CMAKE_CURRENT_BINARY_DIR}/../sstruct_mv/libsstruct_mv_openmpi_hybrid.a
            ${CMAKE_CURRENT_BINARY_DIR}/../krylov/libkrylov_openmpi_hybrid.a
            ${CMAKE_CURRENT_BINARY_DIR}/../utilities/libHYPRE_utilities_openmpi_hybrid.a
            ${OpenMPI_SHARED_LIBRARIES}
            ${OPENMP_LIBS}
            m
        )

        set_target_properties(hybrid-amg2013-openmpi PROPERTIES COMPILE_DEFINITIONS "${OpenMPI_DEFINES};TIMER_USE_MPI=1;HYPRE_USING_OPENMP;HYPRE_LONG_LONG;HYPRE_NO_GLOBAL_PARTITION;HYPRE_TIMING")
        set_target_properties(hybrid-amg2013-openmpi PROPERTIES OUTPUT_NAME "hybrid-amg2013-openmpi-${BUILD_COMPILER_NAME}")

        install(TARGETS hybrid-amg2013-openmpi
            DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
        )
    endif()


endif()
    
if(HAVE_MVAPICH)

    add_executable(mpi-amg2013-mvapich
        ${AMG2013_SOURCES}
    )

    target_include_directories(mpi-amg2013-mvapich PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${Mvapich_INCLUDE_DIRS}
    )

    target_link_libraries(mpi-amg2013-mvapich
	${CMAKE_DL_LIBS}
	${Mvapich_SHARED_LIBRARIES}
	m
    )

    set_target_properties(mpi-amg2013-mvapich PROPERTIES COMPILE_DEFINITIONS "${Mvapich_DEFINES}")
    set_target_properties(mpi-amg2013-mvapich PROPERTIES OUTPUT_NAME "mpi-amg2013-mvapich-${BUILD_COMPILER_NAME}")

    install(TARGETS mpi-amg2013-mvapich
            DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
    )

    # Hybrid OpenMP/MPI executable section -------------------------------------------

    add_executable(hybrid-amg2013-mvapich
        ${AMG2013_SOURCES}
    )

    target_include_directories(hybrid-amg2013-mvapich PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_BINARY_DIR}
        ${OpenMP_INCLUDE_DIRS}
        ${Mvapich_INCLUDE_DIRS}
    )

    target_link_libraries(hybrid-amg2013-mvapich
        ${CMAKE_DL_LIBS}
        ${Mvapich_SHARED_LIBRARIES}
        m
    )

    set_target_properties(hybrid-amg2013-mvapich PROPERTIES COMPILE_DEFINITIONS "${Mvapich_DEFINES};USE_MPI=1")
    set_target_properties(hybrid-amg2013-mvapich PROPERTIES OUTPUT_NAME "hybrid-amg2013-mvapich-${BUILD_COMPILER_NAME}")

    install(TARGETS hybrid-amg2013-mvapich
            DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
    )

endif()

 
if(HAVE_MVAPICH2)

    add_executable(mpi-amg2013-mvapich2
        ${AMG2013_SOURCES}
    )

    target_include_directories(mpi-amg2013-mvapich2 PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${Mvapich2_INCLUDE_DIRS}
    )

    target_link_libraries(mpi-amg2013-mvapich2
	${CMAKE_DL_LIBS}
	${Mvapich2_SHARED_LIBRARIES}
	m
    )

    set_target_properties(mpi-amg2013-mvapich2 PROPERTIES COMPILE_DEFINITIONS "${Mvapich2_DEFINES}")
    set_target_properties(mpi-amg2013-mvapich2 PROPERTIES OUTPUT_NAME "mpi-amg2013-mvapich2-${BUILD_COMPILER_NAME}")

    install(TARGETS mpi-amg2013-mvapich2
            DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
    )

    # Hybrid OpenMP/MPI executable section -------------------------------------------

    add_executable(hybrid-amg2013-mvapich2
        ${AMG2013_SOURCES}
    )

    target_include_directories(hybrid-amg2013-mvapich2 PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_BINARY_DIR}
        ${OpenMP_INCLUDE_DIRS}
        ${Mvapich2_INCLUDE_DIRS}
    )

    target_link_libraries(hybrid-amg2013-mvapich2
        ${CMAKE_DL_LIBS}
        ${Mvapich2_SHARED_LIBRARIES}
        m
    )

    set_target_properties(hybrid-amg2013-mvapich2 PROPERTIES COMPILE_DEFINITIONS "${Mvapich2_DEFINES};USE_MPI=1")
    set_target_properties(hybrid-amg2013-mvapich2 PROPERTIES OUTPUT_NAME "hybrid-amg2013-mvapich2-${BUILD_COMPILER_NAME}")

    install(TARGETS hybrid-amg2013-mvapich2
            DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
    )

endif()


    
if(HAVE_MPT)

    add_executable(mpi-amg2013-mpt
        ${AMG2013_SOURCES}
    )

    target_include_directories(mpi-amg2013-mpt PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${Mpt_INCLUDE_DIRS}
    )

    target_link_libraries(mpi-amg2013-mpt
	${CMAKE_DL_LIBS}
	${Mpt_SHARED_LIBRARIES}
	m
    )

    set_target_properties(mpi-amg2013-mpt PROPERTIES COMPILE_DEFINITIONS "${Mpt_DEFINES}")
    set_target_properties(mpi-amg2013-mpt PROPERTIES OUTPUT_NAME "mpi-amg2013-mpt-${BUILD_COMPILER_NAME}")

    install(TARGETS mpi-amg2013-mpt
            DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
    )

    # Hybrid OpenMP/MPI executable section -------------------------------------------

    add_executable(hybrid-amg2013-mpt
        ${AMG2013_SOURCES}
    )

    target_include_directories(hybrid-amg2013-mpt PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_BINARY_DIR}
        ${OpenMP_INCLUDE_DIRS}
        ${Mpt_INCLUDE_DIRS}
    )

    target_link_libraries(hybrid-amg2013-mpt
        ${CMAKE_DL_LIBS}
        ${Mpt_SHARED_LIBRARIES}
        m
    )

    set_target_properties(hybrid-amg2013-mpt PROPERTIES COMPILE_DEFINITIONS "${Mpt_DEFINES};USE_MPI=1")
    set_target_properties(hybrid-amg2013-mpt PROPERTIES OUTPUT_NAME "hybrid-amg2013-mpt-${BUILD_COMPILER_NAME}")

    install(TARGETS hybrid-amg2013-mpt
            DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
    )


endif()
 
    
if(HAVE_MPICH)

    add_executable(mpi-amg2013-mpich
        ${AMG2013_SOURCES}
    )

    target_include_directories(mpi-amg2013-mpich PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${Mpich_INCLUDE_DIRS}
    )

    target_link_libraries(mpi-amg2013-mpich
	${CMAKE_DL_LIBS}
	${Mpich_SHARED_LIBRARIES}
	m
    )

    set_target_properties(mpi-amg2013-mpich PROPERTIES COMPILE_DEFINITIONS "${Mpich_DEFINES}")
    set_target_properties(mpi-amg2013-mpich PROPERTIES OUTPUT_NAME "mpi-amg2013-mpich-${BUILD_COMPILER_NAME}")

    install(TARGETS mpi-amg2013-mpich
            DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
    )

    # Hybrid OpenMP/MPI executable section -------------------------------------------

    add_executable(hybrid-amg2013-mpich
        ${AMG2013_SOURCES}
    )

    target_include_directories(hybrid-amg2013-mpich PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_BINARY_DIR}
        ${OpenMP_INCLUDE_DIRS}
        ${Mpich_INCLUDE_DIRS}
    )

    target_link_libraries(hybrid-amg2013-mpich
        ${CMAKE_DL_LIBS}
        ${Mpich_SHARED_LIBRARIES}
        m
    )

    set_target_properties(hybrid-amg2013-mpich PROPERTIES COMPILE_DEFINITIONS "${Mpich_DEFINES};USE_MPI=1")
    set_target_properties(hybrid-amg2013-mpich PROPERTIES OUTPUT_NAME "hybrid-amg2013-mpich-${BUILD_COMPILER_NAME}")

    install(TARGETS hybrid-amg2013-mpich
            DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
    )


endif()
 
