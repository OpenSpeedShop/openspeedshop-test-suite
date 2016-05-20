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

set(HPCCG_SOURCES
    main.cpp generate_matrix.cpp read_HPC_row.cpp compute_residual.cpp mytimer.cpp dump_matlab_matrix.cpp 
    HPC_sparsemv.cpp HPCCG.cpp waxpby.cpp ddot.cpp make_local_matrix.cpp exchange_externals.cpp YAML_Element.cpp YAML_Doc.cpp
)

#message(STATUS "HAVE_MPT=" ${HAVE_MPT} )
#message(STATUS "BUILD_COMPILER_NAME=" ${BUILD_COMPILER_NAME} )
#message(STATUS "OpenMP_FLAG_DETECTED=" ${OpenMP_FLAG_DETECTED} )
#message(STATUS "HAVE_OPENMPI=" ${HAVE_OPENMPI} )
#message(STATUS "BUILD_COMPILER_NAME=" ${BUILD_COMPILER_NAME} )
#message(STATUS "OpenMP_RT_NAME=" ${OpenMP_RT_NAME} )

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

    add_executable(mpi-HPCCG-openmpi
        ${HPCCG_SOURCES}
    )

    target_include_directories(mpi-HPCCG-openmpi PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${OpenMPI_INCLUDE_DIRS}
    )

    target_link_libraries(mpi-HPCCG-openmpi
	${CMAKE_DL_LIBS}
	${OpenMPI_SHARED_LIBRARIES}
	m
    )

    set_target_properties(mpi-HPCCG-openmpi PROPERTIES COMPILE_DEFINITIONS "${OpenMPI_DEFINES};USING_MPI=1")
    set_target_properties(mpi-HPCCG-openmpi PROPERTIES OUTPUT_NAME "mpi-HPCCG-openmpi-${BUILD_COMPILER_NAME}")

    install(TARGETS mpi-HPCCG-openmpi
            DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
    )

    # Hybrid OpenMP/MPI executable section -------------------------------------------

    add_executable(hybrid-HPCCG-openmpi
        ${HPCCG_SOURCES}
    )

    target_include_directories(hybrid-HPCCG-openmpi PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_BINARY_DIR}
        ${OpenMP_INCLUDE_DIRS}
        ${OpenMPI_INCLUDE_DIRS}
    )

    target_link_libraries(hybrid-HPCCG-openmpi
        ${CMAKE_DL_LIBS}
        ${OpenMPI_SHARED_LIBRARIES}
        ${OPENMP_LIBS}
        m
    )

    set_target_properties(hybrid-HPCCG-openmpi PROPERTIES COMPILE_DEFINITIONS "${OpenMPI_DEFINES};USING_MPI=1;USING_OMP=1")
    set_target_properties(hybrid-HPCCG-openmpi PROPERTIES OUTPUT_NAME "hybrid-HPCCG-openmpi-${BUILD_COMPILER_NAME}")

    install(TARGETS hybrid-HPCCG-openmpi
            DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
    )


endif()
    
if(HAVE_MVAPICH)

    add_executable(mpi-HPCCG-mvapich
        ${HPCCG_SOURCES}
    )

    target_include_directories(mpi-HPCCG-mvapich PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${Mvapich_INCLUDE_DIRS}
    )

    target_link_libraries(mpi-HPCCG-mvapich
	${CMAKE_DL_LIBS}
	${Mvapich_SHARED_LIBRARIES}
	m
    )

    set_target_properties(mpi-HPCCG-mvapich PROPERTIES COMPILE_DEFINITIONS "${Mvapich_DEFINES};USING_MPI=1")
    set_target_properties(mpi-HPCCG-mvapich PROPERTIES OUTPUT_NAME "mpi-HPCCG-mvapich-${BUILD_COMPILER_NAME}")

    install(TARGETS mpi-HPCCG-mvapich
            DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
    )

    # Hybrid OpenMP/MPI executable section -------------------------------------------

    add_executable(hybrid-HPCCG-mvapich
        ${HPCCG_SOURCES}
    )

    target_include_directories(hybrid-HPCCG-mvapich PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_BINARY_DIR}
        ${OpenMP_INCLUDE_DIRS}
        ${Mvapich_INCLUDE_DIRS}
    )

    target_link_libraries(hybrid-HPCCG-mvapich
        ${CMAKE_DL_LIBS}
        ${Mvapich_SHARED_LIBRARIES}
        m
    )

    set_target_properties(hybrid-HPCCG-mvapich PROPERTIES COMPILE_DEFINITIONS "${Mvapich_DEFINES};USING_MPI=1;USING_OMP=1")
    set_target_properties(hybrid-HPCCG-mvapich PROPERTIES OUTPUT_NAME "hybrid-HPCCG-mvapich-${BUILD_COMPILER_NAME}")

    install(TARGETS hybrid-HPCCG-mvapich
            DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
    )

endif()

 
if(HAVE_MVAPICH2)

    add_executable(mpi-HPCCG-mvapich2
        ${HPCCG_SOURCES}
    )

    target_include_directories(mpi-HPCCG-mvapich2 PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${Mvapich2_INCLUDE_DIRS}
    )

    target_link_libraries(mpi-HPCCG-mvapich2
	${CMAKE_DL_LIBS}
	${Mvapich2_SHARED_LIBRARIES}
	m
    )

    set_target_properties(mpi-HPCCG-mvapich2 PROPERTIES COMPILE_DEFINITIONS "${Mvapich2_DEFINES};USING_MPI=1")
    set_target_properties(mpi-HPCCG-mvapich2 PROPERTIES OUTPUT_NAME "mpi-HPCCG-mvapich2-${BUILD_COMPILER_NAME}")

    install(TARGETS mpi-HPCCG-mvapich2
            DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
    )

    # Hybrid OpenMP/MPI executable section -------------------------------------------

    add_executable(hybrid-HPCCG-mvapich2
        ${HPCCG_SOURCES}
    )

    target_include_directories(hybrid-HPCCG-mvapich2 PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_BINARY_DIR}
        ${OpenMP_INCLUDE_DIRS}
        ${Mvapich2_INCLUDE_DIRS}
    )

    target_link_libraries(hybrid-HPCCG-mvapich2
        ${CMAKE_DL_LIBS}
        ${Mvapich2_SHARED_LIBRARIES}
        m
    )

    set_target_properties(hybrid-HPCCG-mvapich2 PROPERTIES COMPILE_DEFINITIONS "${Mvapich2_DEFINES};USING_MPI=1;USING_OMP=1")
    set_target_properties(hybrid-HPCCG-mvapich2 PROPERTIES OUTPUT_NAME "hybrid-HPCCG-mvapich2-${BUILD_COMPILER_NAME}")

    install(TARGETS hybrid-HPCCG-mvapich2
            DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
    )

endif()


    
if(HAVE_MPT)

    add_executable(mpi-HPCCG-mpt
        ${HPCCG_SOURCES}
    )

    target_include_directories(mpi-HPCCG-mpt PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${Mpt_INCLUDE_DIRS}
    )

    target_link_libraries(mpi-HPCCG-mpt
	${CMAKE_DL_LIBS}
	${Mpt_SHARED_LIBRARIES}
	m
    )

    set_target_properties(mpi-HPCCG-mpt PROPERTIES COMPILE_DEFINITIONS "${Mpt_DEFINES};USING_MPI=1")
    set_target_properties(mpi-HPCCG-mpt PROPERTIES OUTPUT_NAME "mpi-HPCCG-mpt-${BUILD_COMPILER_NAME}")

    install(TARGETS mpi-HPCCG-mpt
            DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
    )

    # Hybrid OpenMP/MPI executable section -------------------------------------------

    add_executable(hybrid-HPCCG-mpt
        ${HPCCG_SOURCES}
    )

    target_include_directories(hybrid-HPCCG-mpt PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_BINARY_DIR}
        ${OpenMP_INCLUDE_DIRS}
        ${Mpt_INCLUDE_DIRS}
    )

    target_link_libraries(hybrid-HPCCG-mpt
        ${CMAKE_DL_LIBS}
        ${Mpt_SHARED_LIBRARIES}
        m
    )

    set_target_properties(hybrid-HPCCG-mpt PROPERTIES COMPILE_DEFINITIONS "${Mpt_DEFINES};USING_OMP=1")
    set_target_properties(hybrid-HPCCG-mpt PROPERTIES OUTPUT_NAME "hybrid-HPCCG-mpt-${BUILD_COMPILER_NAME}")

    install(TARGETS hybrid-HPCCG-mpt
            DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
    )


endif()
 
    
if(HAVE_MPICH)

    add_executable(mpi-HPCCG-mpich
        ${HPCCG_SOURCES}
    )

    target_include_directories(mpi-HPCCG-mpich PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${Mpich_INCLUDE_DIRS}
    )

    target_link_libraries(mpi-HPCCG-mpich
	${CMAKE_DL_LIBS}
	${Mpich_SHARED_LIBRARIES}
	m
    )

    set_target_properties(mpi-HPCCG-mpich PROPERTIES COMPILE_DEFINITIONS "${Mpich_DEFINES};USING_MPI=1")
    set_target_properties(mpi-HPCCG-mpich PROPERTIES OUTPUT_NAME "mpi-HPCCG-mpich-${BUILD_COMPILER_NAME}")

    install(TARGETS mpi-HPCCG-mpich
            DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
    )

    # Hybrid OpenMP/MPI executable section -------------------------------------------

    add_executable(hybrid-HPCCG-mpich
        ${HPCCG_SOURCES}
    )

    target_include_directories(hybrid-HPCCG-mpich PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_BINARY_DIR}
        ${OpenMP_INCLUDE_DIRS}
        ${Mpich_INCLUDE_DIRS}
    )

    target_link_libraries(hybrid-HPCCG-mpich
        ${CMAKE_DL_LIBS}
        ${Mpich_SHARED_LIBRARIES}
        m
    )

    set_target_properties(hybrid-HPCCG-mpich PROPERTIES COMPILE_DEFINITIONS "${Mpich_DEFINES};USING_MPI=1;USING_OMP=1")
    set_target_properties(hybrid-HPCCG-mpich PROPERTIES OUTPUT_NAME "hybrid-HPCCG-mpich-${BUILD_COMPILER_NAME}")

    install(TARGETS hybrid-HPCCG-mpich
            DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
    )

endif()
 
