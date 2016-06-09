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

set(UTILITIES_SOURCES
    amg_linklist.h exchange_data.h general.h hypre_memory.h hypre_smp_forloop.h HYPRE_utilities.h mpistubs.h threading.h thread_mpistubs.h timing.h umalloc_local.h utilities.h amg_linklist.c binsearch.c exchange_data.c hypre_memory.c hypre_qsort.c memory_dmalloc.c mpistubs.c qsplit.c random.c threading.c thread_mpistubs.c timer.c timing.c umalloc_local.c hypre_error.c 
)

if(HAVE_OPENMPI)

    if(OpenMP_FLAG_DETECTED)

        # Hybrid OpenMP/MPI library section -------------------------------------------

        add_library(HYPRE_utilities_openmpi_hybrid STATIC
            ${UTILITIES_SOURCES}
        )

        target_include_directories(HYPRE_utilities_openmpi_hybrid PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}
            ${CMAKE_CURRENT_SOURCE_DIR}/..
            ${CMAKE_CURRENT_BINARY_DIR}
            ${OpenMP_INCLUDE_DIRS}
            ${OpenMPI_INCLUDE_DIRS}
        )

        target_link_libraries(HYPRE_utilities_openmpi_hybrid
            ${CMAKE_DL_LIBS}
            ${OpenMPI_SHARED_LIBRARIES}
            ${OPENMP_LIBS}
            m
        )

        add_custom_command (
            OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/libHYPRE_utilities_openmpi_hybrid.a
            TARGET ${TARGET_NAME} POST_BUILD
            COMMAND ${CMAKE_AR} -rcu ${CMAKE_CURRENT_BINARY_DIR}/*.o* ${CMAKE_CURRENT_BINARY_DIR}/libHYPRE_utilities_openmpi_hybrid.a
            COMMAND ${CMAKE_RANLIB} ${CMAKE_CURRENT_BINARY_DIR}/libHYPRE_utilities_openmpi_hybrid.a
            DEPENDS HYPRE_utilities_openmpi_hybrid
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        )

        #add_custom_target(runHYPRE_utilities_openmpi_hybrid ALL DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/libHYPRE_utilities_openmpi_hybrid.a)
        add_custom_target(runHYPRE_utilities_openmpi_hybrid ALL DEPENDS HYPRE_utilities_openmpi_hybrid)

        set_target_properties(HYPRE_utilities_openmpi_hybrid PROPERTIES COMPILE_DEFINITIONS "${OpenMPI_DEFINES};TIMER_USE_MPI=1;HYPRE_USING_OPENMP;HYPRE_LONG_LONG;HYPRE_NO_GLOBAL_PARTITION;HYPRE_TIMING")
        set_target_properties(HYPRE_utilities_openmpi_hybrid PROPERTIES OUTPUT_NAME "HYPRE_utilities_openmpi_hybrid")

        #add_custom_command (
        #    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/HYPRE_utilities_openmpi_hybrid.a
        #    TARGET ${TARGET_NAME} POST_BUILD 
        #    COMMAND ${CMAKE_AR} -rcu ${CMAKE_CURRENT_BINARY_DIR}/*.o* ${CMAKE_CURRENT_BINARY_DIR}/HYPRE_utilities_openmpi_hybrid.a
        #    COMMAND ${CMAKE_RANLIB} ${CMAKE_CURRENT_BINARY_DIR}/HYPRE_utilities_openmpi_hybrid.a
        #    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/HYPRE_utilities_openmpi_hybrid.a
        #    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        #)
        #add_custom_command (
        #    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/HYPRE_utilities_openmpi_hybrid.a
        #    TARGET ${TARGET_NAME} POST_BUILD
        #    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/HYPRE_utilities_openmpi_hybrid.a
        #    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        # )

        install(TARGETS HYPRE_utilities_openmpi_hybrid ARCHIVE
                DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
        )

    endif()

    # MPI library only section -------------------------------------------

    add_library(HYPRE_utilities_openmpi_mpi STATIC
        ${UTILITIES_SOURCES}
    )

    target_include_directories(HYPRE_utilities_openmpi_mpi PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/..
        ${CMAKE_CURRENT_BINARY_DIR}
        ${OpenMPI_INCLUDE_DIRS}
    )

    target_link_libraries(HYPRE_utilities_openmpi_mpi
        ${CMAKE_DL_LIBS}
        ${OpenMPI_SHARED_LIBRARIES}
        m
    )

    add_custom_command (
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/libHYPRE_utilities_openmpi_mpi.a
        TARGET ${TARGET_NAME} POST_BUILD
        COMMAND ${CMAKE_AR} -rcu ${CMAKE_CURRENT_BINARY_DIR}/*.o* ${CMAKE_CURRENT_BINARY_DIR}/libHYPRE_utilities_openmpi_mpi.a
        COMMAND ${CMAKE_RANLIB} ${CMAKE_CURRENT_BINARY_DIR}/libHYPRE_utilities_openmpi_mpi.a
        DEPENDS HYPRE_utilities_openmpi_mpi
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )

    #add_custom_target(runHYPRE_utilities_openmpi_mpi ALL DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/libHYPRE_utilities_openmpi_mpi.a)
    add_custom_target(runHYPRE_utilities_openmpi_mpi ALL DEPENDS HYPRE_utilities_openmpi_mpi)

    set_target_properties(HYPRE_utilities_openmpi_mpi PROPERTIES COMPILE_DEFINITIONS "${OpenMPI_DEFINES};TIMER_USE_MPI;HYPRE_LONG_LONG;HYPRE_NO_GLOBAL_PARTITION;HYPRE_TIMING")
    set_target_properties(HYPRE_utilities_openmpi_mpi PROPERTIES OUTPUT_NAME "HYPRE_utilities_openmpi_mpi")

    install(TARGETS HYPRE_utilities_openmpi_mpi ARCHIVE
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
    )

endif()
 

if(HAVE_MPT)

    if(OpenMP_FLAG_DETECTED)

        # Hybrid OpenMP/MPI library section -------------------------------------------

        add_library(HYPRE_utilities_mpt_hybrid STATIC
            ${UTILITIES_SOURCES}
        )

        target_include_directories(HYPRE_utilities_mpt_hybrid PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}
            ${CMAKE_CURRENT_SOURCE_DIR}/..
            ${CMAKE_CURRENT_BINARY_DIR}
            ${OpenMP_INCLUDE_DIRS}
            ${Mpt_INCLUDE_DIRS}
        )

        target_link_libraries(HYPRE_utilities_mpt_hybrid
            ${CMAKE_DL_LIBS}
            ${Mpt_SHARED_LIBRARIES}
            ${OPENMP_LIBS}
            m
        )

        set_target_properties(HYPRE_utilities_mpt_hybrid PROPERTIES COMPILE_DEFINITIONS "${Mpt_DEFINES};TIMER_USE_MPI=1;HYPRE_USING_OPENMP;HYPRE_LONG_LONG;HYPRE_NO_GLOBAL_PARTITION;HYPRE_TIMING")
        set_target_properties(HYPRE_utilities_mpt_hybrid PROPERTIES OUTPUT_NAME "HYPRE_utilities_mpt_hybrid")

        install(TARGETS HYPRE_utilities_mpt_hybrid ARCHIVE
                DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
        )

    endif()

    # MPI library only section -------------------------------------------

    add_library(HYPRE_utilities_mpt_mpi STATIC
        ${UTILITIES_SOURCES}
    )

    target_include_directories(HYPRE_utilities_mpt_mpi PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/..
        ${CMAKE_CURRENT_BINARY_DIR}
        ${Mpt_INCLUDE_DIRS}
    )

    target_link_libraries(HYPRE_utilities_mpt_mpi
        ${CMAKE_DL_LIBS}
        ${Mpt_SHARED_LIBRARIES}
        m
    )

    set_target_properties(HYPRE_utilities_mpt_mpi PROPERTIES COMPILE_DEFINITIONS "${Mpt_DEFINES};TIMER_USE_MPI;HYPRE_LONG_LONG;HYPRE_NO_GLOBAL_PARTITION;HYPRE_TIMING")
    set_target_properties(HYPRE_utilities_mpt_mpi PROPERTIES OUTPUT_NAME "HYPRE_utilities_mpt_mpi")

    install(TARGETS HYPRE_utilities_mpt_mpi ARCHIVE
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
    )

endif()

if(HAVE_MPICH)

    if(OpenMP_FLAG_DETECTED)

        # Hybrid OpenMP/MPI library section -------------------------------------------

        add_library(HYPRE_utilities_mpich_hybrid STATIC
            ${UTILITIES_SOURCES}
        )

        target_include_directories(HYPRE_utilities_mpich_hybrid PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}
            ${CMAKE_CURRENT_SOURCE_DIR}/..
            ${CMAKE_CURRENT_BINARY_DIR}
            ${OpenMP_INCLUDE_DIRS}
            ${Mpich_INCLUDE_DIRS}
        )

        target_link_libraries(HYPRE_utilities_mpich_hybrid
            ${CMAKE_DL_LIBS}
            ${Mpich_SHARED_LIBRARIES}
            ${OPENMP_LIBS}
            m
        )

        set_target_properties(HYPRE_utilities_mpich_hybrid PROPERTIES COMPILE_DEFINITIONS "${Mpich_DEFINES};TIMER_USE_MPI=1;HYPRE_USING_OPENMP;HYPRE_LONG_LONG;HYPRE_NO_GLOBAL_PARTITION;HYPRE_TIMING")
        set_target_properties(HYPRE_utilities_mpich_hybrid PROPERTIES OUTPUT_NAME "HYPRE_utilities_mpich_hybrid")

        install(TARGETS HYPRE_utilities_mpich_hybrid ARCHIVE
                DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
        )

    endif()

    # MPI library only section -------------------------------------------

    add_library(HYPRE_utilities_mpich_mpi STATIC
        ${UTILITIES_SOURCES}
    )

    target_include_directories(HYPRE_utilities_mpich_mpi PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/..
        ${CMAKE_CURRENT_BINARY_DIR}
        ${Mpich_INCLUDE_DIRS}
    )

    target_link_libraries(HYPRE_utilities_mpich_mpi
        ${CMAKE_DL_LIBS}
        ${Mpich_SHARED_LIBRARIES}
        m
    )

    set_target_properties(HYPRE_utilities_mpich_mpi PROPERTIES COMPILE_DEFINITIONS "${Mpich_DEFINES};TIMER_USE_MPI;HYPRE_LONG_LONG;HYPRE_NO_GLOBAL_PARTITION;HYPRE_TIMING")
    set_target_properties(HYPRE_utilities_mpich_mpi PROPERTIES OUTPUT_NAME "HYPRE_utilities_mpich_mpi")

    install(TARGETS HYPRE_utilities_mpich_mpi ARCHIVE
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
    )

endif()
 

if(HAVE_MVAPICH)

    if(OpenMP_FLAG_DETECTED)

        # Hybrid OpenMP/MPI library section -------------------------------------------

        add_library(HYPRE_utilities_mvapich_hybrid STATIC
            ${UTILITIES_SOURCES}
        )

        target_include_directories(HYPRE_utilities_mvapich_hybrid PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}
            ${CMAKE_CURRENT_SOURCE_DIR}/..
            ${CMAKE_CURRENT_BINARY_DIR}
            ${OpenMP_INCLUDE_DIRS}
            ${Mvapich_INCLUDE_DIRS}
        )

        target_link_libraries(HYPRE_utilities_mvapich_hybrid
            ${CMAKE_DL_LIBS}
            ${Mvapich_SHARED_LIBRARIES}
            ${OPENMP_LIBS}
            m
        )

        set_target_properties(HYPRE_utilities_mvapich_hybrid PROPERTIES COMPILE_DEFINITIONS "${Mvapich_DEFINES};TIMER_USE_MPI=1;HYPRE_USING_OPENMP;HYPRE_LONG_LONG;HYPRE_NO_GLOBAL_PARTITION;HYPRE_TIMING")
        set_target_properties(HYPRE_utilities_mvapich_hybrid PROPERTIES OUTPUT_NAME "HYPRE_utilities_mvapich_hybrid")

        install(TARGETS HYPRE_utilities_mvapich_hybrid ARCHIVE
                DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
        )

    endif()

    # MPI library only section -------------------------------------------

    add_library(HYPRE_utilities_mvapich_mpi STATIC
        ${UTILITIES_SOURCES}
    )

    target_include_directories(HYPRE_utilities_mvapich_mpi PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/..
        ${CMAKE_CURRENT_BINARY_DIR}
        ${Mvapich_INCLUDE_DIRS}
    )

    target_link_libraries(HYPRE_utilities_mvapich_mpi
        ${CMAKE_DL_LIBS}
        ${Mvapich_SHARED_LIBRARIES}
        m
    )

    set_target_properties(HYPRE_utilities_mvapich_mpi PROPERTIES COMPILE_DEFINITIONS "${Mvapich_DEFINES};TIMER_USE_MPI;HYPRE_LONG_LONG;HYPRE_NO_GLOBAL_PARTITION;HYPRE_TIMING")
    set_target_properties(HYPRE_utilities_mvapich_mpi PROPERTIES OUTPUT_NAME "HYPRE_utilities_mvapich_mpi")

    install(TARGETS HYPRE_utilities_mvapich_mpi ARCHIVE
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
    )

endif()
 
 

if(HAVE_MVAPICH2)

    if(OpenMP_FLAG_DETECTED)

        # Hybrid OpenMP/MPI library section -------------------------------------------

        add_library(HYPRE_utilities_mvapich2_hybrid STATIC
            ${UTILITIES_SOURCES}
        )

        target_include_directories(HYPRE_utilities_mvapich2_hybrid PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}
            ${CMAKE_CURRENT_SOURCE_DIR}/..
            ${CMAKE_CURRENT_BINARY_DIR}
            ${OpenMP_INCLUDE_DIRS}
            ${Mvapich2_INCLUDE_DIRS}
        )

        target_link_libraries(HYPRE_utilities_mvapich2_hybrid
            ${CMAKE_DL_LIBS}
            ${Mvapich2_SHARED_LIBRARIES}
            ${OPENMP_LIBS}
            m
        )

        set_target_properties(HYPRE_utilities_mvapich2_hybrid PROPERTIES COMPILE_DEFINITIONS "${Mvapich2_DEFINES};TIMER_USE_MPI=1;HYPRE_USING_OPENMP;HYPRE_LONG_LONG;HYPRE_NO_GLOBAL_PARTITION;HYPRE_TIMING")
        set_target_properties(HYPRE_utilities_mvapich2_hybrid PROPERTIES OUTPUT_NAME "HYPRE_utilities_mvapich2_hybrid")

        install(TARGETS HYPRE_utilities_mvapich2_hybrid ARCHIVE
                DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
        )

    endif()

    # MPI library only section -------------------------------------------

    add_library(HYPRE_utilities_mvapich2_mpi STATIC
        ${UTILITIES_SOURCES}
    )

    target_include_directories(HYPRE_utilities_mvapich2_mpi PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/..
        ${CMAKE_CURRENT_BINARY_DIR}
        ${Mvapich2_INCLUDE_DIRS}
    )

    target_link_libraries(HYPRE_utilities_mvapich2_mpi
        ${CMAKE_DL_LIBS}
        ${Mvapich2_SHARED_LIBRARIES}
        m
    )

    set_target_properties(HYPRE_utilities_mvapich2_mpi PROPERTIES COMPILE_DEFINITIONS "${Mvapich2_DEFINES};TIMER_USE_MPI;HYPRE_LONG_LONG;HYPRE_NO_GLOBAL_PARTITION;HYPRE_TIMING")
    set_target_properties(HYPRE_utilities_mvapich2_mpi PROPERTIES OUTPUT_NAME "HYPRE_utilities_mvapich2_mpi")

    install(TARGETS HYPRE_utilities_mvapich2_mpi ARCHIVE
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
    )

endif()
 
 
