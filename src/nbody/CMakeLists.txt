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

#	${Mvapich2_INCLUDE_DIRS}
if(HAVE_OPENMPI)

    add_executable(mpi-nbody-openmpi
        ${NBODY_SOURCES}
    )

    target_include_directories(mpi-nbody-openmpi PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${OpenMPI_INCLUDE_DIRS}
    )

    target_link_libraries(mpi-nbody-openmpi
	${CMAKE_DL_LIBS}
	${OpenMPI_SHARED_LIBRARIES}
	m
    )

    set_target_properties(mpi-nbody-openmpi PROPERTIES COMPILE_DEFINITIONS "${OpenMPI_DEFINES}")
    set_target_properties(mpi-nbody-openmpi PROPERTIES OUTPUT_NAME "mpi-nbody-openmpi-${BUILD_COMPILER_NAME}")

    install(TARGETS mpi-nbody-openmpi
            DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
    )
endif()
    
if(HAVE_MVAPICH2)

    add_executable(mpi-nbody-mvapich2
        ${NBODY_SOURCES}
    )

    target_include_directories(mpi-nbody-mvapich2 PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${Mvapich2_INCLUDE_DIRS}
    )

    target_link_libraries(mpi-nbody-mvapich2
	${CMAKE_DL_LIBS}
	${Mvapich2_SHARED_LIBRARIES}
	m
    )

    set_target_properties(mpi-nbody-mvapich2 PROPERTIES COMPILE_DEFINITIONS "${Mvapich2_DEFINES}")
    set_target_properties(mpi-nbody-mvapich2 PROPERTIES OUTPUT_NAME "mpi-nbody-mvapich2-${BUILD_COMPILER_NAME}")

    install(TARGETS mpi-nbody-mvapich2
            DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
    )

endif()
    
if(HAVE_MPT)

    add_executable(mpi-nbody-mpt
        ${NBODY_SOURCES}
    )

    target_include_directories(mpi-nbody-mpt PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${Mpt_INCLUDE_DIRS}
    )

    target_link_libraries(mpi-nbody-mpt
	${CMAKE_DL_LIBS}
	${Mpt_SHARED_LIBRARIES}
	m
    )

    set_target_properties(mpi-nbody-mpt PROPERTIES COMPILE_DEFINITIONS "${Mpt_DEFINES}")
    set_target_properties(mpi-nbody-mpt PROPERTIES OUTPUT_NAME "mpi-nbody-mpt-${BUILD_COMPILER_NAME}")

    install(TARGETS mpi-nbody-mpt
            DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
    )

endif()
    
    # Create and install static library

    #set_target_properties(mpi-openmpi-rt-offline PROPERTIES PREFIX "")

    #add_library(mpi-openmpi-offline STATIC
    #    ${RT_SOURCES}
#	offline.c
#	wrappers.c
#	wrappers-fortran.c
#    )

#    target_include_directories(mpi-openmpi-offline PUBLIC
#	${CMAKE_CURRENT_SOURCE_DIR}
#	${CMAKE_CURRENT_BINARY_DIR}
#	${PROJECT_SOURCE_DIR}/libopenss-runtime
#	${PROJECT_BUILD_DIR}/libopenss-runtime
#	${PROJECT_SOURCE_DIR}/libopenss-runtime/offline
#	${LibMonitor_INCLUDE_DIRS}
#	${LibUnwind_INCLUDE_DIRS}
#	${OpenMPI_INCLUDE_DIRS}
#        ${CMAKE_CURRENT_BINARY_DIR}/../../../libopenss-runtime
#    )
#
#    target_link_libraries(mpi-openmpi-offline
#	${CMAKE_DL_LIBS}
#	rt
#	pthread
#        -Wl,-whole-archive
#	openss-runtime-offline
#	openss-runtime-unwind
#        -Wl,-no-whole-archive
#	-Bdynamic
#	${LibUnwind_LIBRARY_SHARED}
#    )
#
#
#    #set_target_properties(mpi-openmpi-offline PROPERTIES VERSION 1.1.0)
#    set_target_properties(mpi-openmpi-offline PROPERTIES COMPILE_DEFINITIONS "${TLS_DEFINES};${LibUnwind_DEFINES};${OpenMPI_DEFINES};OPENSS_STATIC") 
#
#    install(TARGETS mpi-openmpi-offline
#	ARCHIVE DESTINATION lib${LIB_SUFFIX}/openspeedshop
#    )
#

#if(HAVE_MPICH2)
#
#    add_library(mpi-mpich2-rt-offline SHARED
#        ${RT_SOURCES}
#	offline.c
#	wrappers.c
#	wrappers-fortran.c
#    )
#
#    target_include_directories(mpi-mpich2-rt-offline PUBLIC
#	${CMAKE_CURRENT_SOURCE_DIR}
#	${CMAKE_CURRENT_BINARY_DIR}
#	${PROJECT_SOURCE_DIR}/libopenss-runtime
#	${PROJECT_BUILD_DIR}/libopenss-runtime
#	${PROJECT_SOURCE_DIR}/libopenss-runtime/offline
#	${LibMonitor_INCLUDE_DIRS}
#	${LibUnwind_INCLUDE_DIRS}
#	${Mpich2_INCLUDE_DIRS}
#    )
#
#    target_link_libraries(mpi-mpich2-rt-offline
#	${CMAKE_DL_LIBS}
#	rt
#	pthread
#        -Wl,-whole-archive
#	openss-runtime-offline
#	openss-runtime-unwind
#        -Wl,-no-whole-archive
#	-Bdynamic
#	${LibUnwind_LIBRARY_SHARED}
#    )
#
#
#    #set_target_properties(mpi-mpich2-rt-offline PROPERTIES VERSION 1.1.0)
#    set_target_properties(mpi-mpich2-rt-offline PROPERTIES COMPILE_DEFINITIONS "${TLS_DEFINES};${LibUnwind_DEFINES};${Mpich2_DEFINES}") 
#
#    install(TARGETS mpi-mpich2-rt-offline
#	LIBRARY DESTINATION lib${LIB_SUFFIX}/openspeedshop
#    )
#
#    set_target_properties(mpi-mpich2-rt-offline PROPERTIES PREFIX "")
#
#    add_library(mpi-mpich2-offline STATIC
#        ${RT_SOURCES}
#	offline.c
#	wrappers.c
#	wrappers-fortran.c
#    )
#
#    target_include_directories(mpi-mpich2-offline PUBLIC
#	${CMAKE_CURRENT_SOURCE_DIR}
#	${CMAKE_CURRENT_BINARY_DIR}
#	${PROJECT_SOURCE_DIR}/libopenss-runtime
#	${PROJECT_BUILD_DIR}/libopenss-runtime
#	${PROJECT_SOURCE_DIR}/libopenss-runtime/offline
#	${LibMonitor_INCLUDE_DIRS}
#	${LibUnwind_INCLUDE_DIRS}
#	${Mpich2_INCLUDE_DIRS}
#    )
#
#
#    target_link_libraries(mpi-mpich2-offline
#	${CMAKE_DL_LIBS}
#	rt
#	pthread
#        -Wl,-whole-archive
#	openss-runtime-offline
#	openss-runtime-unwind
#        -Wl,-no-whole-archive
#	-Bdynamic
#	${LibUnwind_LIBRARY_SHARED}
#    )
#
#
#    #set_target_properties(mpi-mpich2-offline PROPERTIES VERSION 1.1.0)
#    set_target_properties(mpi-mpich2-offline PROPERTIES COMPILE_DEFINITIONS "${TLS_DEFINES};${LibUnwind_DEFINES};${Mpich2_DEFINES};OPENSS_STATIC") 
#
#    install(TARGETS mpi-mpich2-offline
#	ARCHIVE DESTINATION lib${LIB_SUFFIX}/openspeedshop
#    )
#
#
#endif()
#
#if(HAVE_MPICH)
#
#    add_library(mpi-mpich-rt-offline SHARED
#        ${RT_SOURCES}
#	offline.c
#	wrappers.c
#	wrappers-fortran.c
#    )
#
#    target_include_directories(mpi-mpich-rt-offline PUBLIC
#	${CMAKE_CURRENT_SOURCE_DIR}
#	${CMAKE_CURRENT_BINARY_DIR}
#	${PROJECT_SOURCE_DIR}/libopenss-runtime
#	${PROJECT_BUILD_DIR}/libopenss-runtime
#	${PROJECT_SOURCE_DIR}/libopenss-runtime/offline
#	${LibMonitor_INCLUDE_DIRS}
#	${LibUnwind_INCLUDE_DIRS}
#	${Mpich_INCLUDE_DIRS}
#    )
#
#    target_link_libraries(mpi-mpich-rt-offline
#	${CMAKE_DL_LIBS}
#	rt
#	pthread
#        -Wl,-whole-archive
#	openss-runtime-offline
#	openss-runtime-unwind
#        -Wl,-no-whole-archive
#	-Bdynamic
#	${LibUnwind_LIBRARY_SHARED}
#    )
#
#
#    #set_target_properties(mpi-mpich-rt-offline PROPERTIES VERSION 1.1.0)
#    set_target_properties(mpi-mpich-rt-offline PROPERTIES COMPILE_DEFINITIONS "${TLS_DEFINES};${LibUnwind_DEFINES};${Mpich_DEFINES}") 
#
#    install(TARGETS mpi-mpich-rt-offline
#	LIBRARY DESTINATION lib${LIB_SUFFIX}/openspeedshop
#    )
#
#    set_target_properties(mpi-mpich-rt-offline PROPERTIES PREFIX "")
#
#    # Create and install static version
#
#    add_library(mpi-mpich-offline STATIC
#        ${RT_SOURCES}
#	offline.c
#	wrappers.c
#	wrappers-fortran.c
#    )
#
#    target_include_directories(mpi-mpich-offline PUBLIC
#	${CMAKE_CURRENT_SOURCE_DIR}
#	${CMAKE_CURRENT_BINARY_DIR}
#	${PROJECT_SOURCE_DIR}/libopenss-runtime
#	${PROJECT_BUILD_DIR}/libopenss-runtime
#	${PROJECT_SOURCE_DIR}/libopenss-runtime/offline
#	${LibMonitor_INCLUDE_DIRS}
#	${LibUnwind_INCLUDE_DIRS}
#	${Mpich_INCLUDE_DIRS}
#    )
#
#
#    target_link_libraries(mpi-mpich-offline
#	${CMAKE_DL_LIBS}
#	rt
#	pthread
#        -Wl,-whole-archive
#	openss-runtime-offline
#	openss-runtime-unwind
#        -Wl,-no-whole-archive
#	-Bdynamic
#	${LibUnwind_LIBRARY_SHARED}
#    )
#
#
#    #set_target_properties(mpi-mpich-offline PROPERTIES VERSION 1.1.0)
#    set_target_properties(mpi-mpich-offline PROPERTIES COMPILE_DEFINITIONS "${TLS_DEFINES};${LibUnwind_DEFINES};${Mpich_DEFINES};OPENSS_STATIC") 
#
#    install(TARGETS mpi-mpich-offline
#	ARCHIVE DESTINATION lib${LIB_SUFFIX}/openspeedshop
#    )
#
#endif()
#
#if(HAVE_MPT)
#
#    add_library(mpi-mpt-rt-offline SHARED
#        ${RT_SOURCES}
#	offline.c
#	wrappers.c
#	wrappers-fortran.c
#    )
#
#    target_include_directories(mpi-mpt-rt-offline PUBLIC
#	${CMAKE_CURRENT_SOURCE_DIR}
#	${CMAKE_CURRENT_BINARY_DIR}
#	${PROJECT_SOURCE_DIR}/libopenss-runtime
#	${PROJECT_BUILD_DIR}/libopenss-runtime
#	${PROJECT_SOURCE_DIR}/libopenss-runtime/offline
#	${LibMonitor_INCLUDE_DIRS}
#	${LibUnwind_INCLUDE_DIRS}
#	${Mpt_INCLUDE_DIRS}
#    )
#
#    target_link_libraries(mpi-mpt-rt-offline
#	${CMAKE_DL_LIBS}
#	rt
#	pthread
#        -Wl,-whole-archive
#	openss-runtime-offline
#	openss-runtime-unwind
#        -Wl,-no-whole-archive
#	-Bdynamic
#	${LibUnwind_LIBRARY_SHARED}
#    )
#
#
#    #set_target_properties(mpi-mpt-rt-offline PROPERTIES VERSION 1.1.0)
#    set_target_properties(mpi-mpt-rt-offline PROPERTIES COMPILE_DEFINITIONS "${TLS_DEFINES};${LibUnwind_DEFINES};${Mpt_DEFINES};${SGIMPT_DEFINES}") 
#
#    install(TARGETS mpi-mpt-rt-offline
#	LIBRARY DESTINATION lib${LIB_SUFFIX}/openspeedshop
#    )
#
#    set_target_properties(mpi-mpt-rt-offline PROPERTIES PREFIX "")
#
#    # Create and install static version
#
#    add_library(mpi-mpt-offline STATIC
#        ${RT_SOURCES}
#	offline.c
#	wrappers.c
#	wrappers-fortran.c
#    )
#
#    target_include_directories(mpi-mpt-offline PUBLIC
#	${CMAKE_CURRENT_SOURCE_DIR}
#	${CMAKE_CURRENT_BINARY_DIR}
#	${PROJECT_SOURCE_DIR}/libopenss-runtime
#	${PROJECT_BUILD_DIR}/libopenss-runtime
#	${PROJECT_SOURCE_DIR}/libopenss-runtime/offline
#	${LibMonitor_INCLUDE_DIRS}
#	${LibUnwind_INCLUDE_DIRS}
#	${Mpt_INCLUDE_DIRS}
#    )
#
#
#    target_link_libraries(mpi-mpt-offline
#	${CMAKE_DL_LIBS}
#	rt
#	pthread
#        -Wl,-whole-archive
#	openss-runtime-offline
#	openss-runtime-unwind
#        -Wl,-no-whole-archive
#	-Bdynamic
#	${LibUnwind_LIBRARY_SHARED}
#    )
#
#
#    #set_target_properties(mpi-mpt-offline PROPERTIES VERSION 1.1.0)
#    set_target_properties(mpi-mpt-offline PROPERTIES COMPILE_DEFINITIONS "${TLS_DEFINES};${LibUnwind_DEFINES};${Mpt_DEFINES};${SGIMPT_DEFINES};OPENSS_STATIC") 
#
#    install(TARGETS mpi-mpt-offline
#	ARCHIVE DESTINATION lib${LIB_SUFFIX}/openspeedshop
#    )
#
#
#endif()
#
#if(HAVE_MVAPICH2)
#
#    add_library(mpi-mvapich2-rt-offline SHARED
#        ${RT_SOURCES}
#	offline.c
#	wrappers.c
#	wrappers-fortran.c
#    )
#
#    target_include_directories(mpi-mvapich2-rt-offline PUBLIC
#	${CMAKE_CURRENT_SOURCE_DIR}
#	${CMAKE_CURRENT_BINARY_DIR}
#	${PROJECT_SOURCE_DIR}/libopenss-runtime
#	${PROJECT_BUILD_DIR}/libopenss-runtime
#	${PROJECT_SOURCE_DIR}/libopenss-runtime/offline
#	${LibMonitor_INCLUDE_DIRS}
#	${LibUnwind_INCLUDE_DIRS}
#	${Mvapich2_INCLUDE_DIRS}
#    )
#
#    target_link_libraries(mpi-mvapich2-rt-offline
#	${CMAKE_DL_LIBS}
#	rt
#	pthread
#        -Wl,-whole-archive
#	openss-runtime-offline
#	openss-runtime-unwind
#        -Wl,-no-whole-archive
#	-Bdynamic
#	${LibUnwind_LIBRARY_SHARED}
#    )
#
#
#    #set_target_properties(mpi-mvapich2-rt-offline PROPERTIES VERSION 1.1.0)
#    set_target_properties(mpi-mvapich2-rt-offline PROPERTIES COMPILE_DEFINITIONS "${TLS_DEFINES};${LibUnwind_DEFINES};${Mvapich2_DEFINES}") 
#
#    install(TARGETS mpi-mvapich2-rt-offline
#	LIBRARY DESTINATION lib${LIB_SUFFIX}/openspeedshop
#    )
#
#    set_target_properties(mpi-mvapich2-rt-offline PROPERTIES PREFIX "")
#
#    # Create and install static version
#
#    add_library(mpi-mvapich2-offline STATIC
#        ${RT_SOURCES}
#	offline.c
#	wrappers.c
#	wrappers-fortran.c
#    )
#
#    target_include_directories(mpi-mvapich2-offline PUBLIC
#	${CMAKE_CURRENT_SOURCE_DIR}
#	${CMAKE_CURRENT_BINARY_DIR}
#	${PROJECT_SOURCE_DIR}/libopenss-runtime
#	${PROJECT_BUILD_DIR}/libopenss-runtime
#	${PROJECT_SOURCE_DIR}/libopenss-runtime/offline
#	${LibMonitor_INCLUDE_DIRS}
#	${LibUnwind_INCLUDE_DIRS}
#	${Mvapich2_INCLUDE_DIRS}
#    )
#
#
#    target_link_libraries(mpi-mvapich2-offline
#	${CMAKE_DL_LIBS}
#	rt
#	pthread
#        -Wl,-whole-archive
#	openss-runtime-offline
#	openss-runtime-unwind
#        -Wl,-no-whole-archive
#	-Bdynamic
#	${LibUnwind_LIBRARY_SHARED}
#    )
#
#
#    #set_target_properties(mpi-mvapich2-offline PROPERTIES VERSION 1.1.0)
#    set_target_properties(mpi-mvapich2-offline PROPERTIES COMPILE_DEFINITIONS "${TLS_DEFINES};${LibUnwind_DEFINES};${Mvapich2_DEFINES};OPENSS_STATIC") 
#
#    install(TARGETS mpi-mvapich2-offline
#	ARCHIVE DESTINATION lib${LIB_SUFFIX}/openspeedshop
#    )
#
#
#endif()
#
#if(HAVE_MVAPICH)
#
#    add_library(mpi-mvapich-rt-offline SHARED
#        ${RT_SOURCES}
#	offline.c
#	wrappers.c
#	wrappers-fortran.c
#    )
#
#    target_include_directories(mpi-mvapich-rt-offline PUBLIC
#	${CMAKE_CURRENT_SOURCE_DIR}
#	${CMAKE_CURRENT_BINARY_DIR}
#	${PROJECT_SOURCE_DIR}/libopenss-runtime
#	${PROJECT_BUILD_DIR}/libopenss-runtime
#	${PROJECT_SOURCE_DIR}/libopenss-runtime/offline
#	${LibMonitor_INCLUDE_DIRS}
#	${LibUnwind_INCLUDE_DIRS}
#	${Mvapich_INCLUDE_DIRS}
#    )
#
#    target_link_libraries(mpi-mvapich-rt-offline
#	${CMAKE_DL_LIBS}
#	rt
#	pthread
#        -Wl,-whole-archive
#	openss-runtime-offline
#	openss-runtime-unwind
#        -Wl,-no-whole-archive
#	-Bdynamic
#	${LibUnwind_LIBRARY_SHARED}
#    )
#
#
#    #set_target_properties(mpi-mvapich-rt-offline PROPERTIES VERSION 1.1.0)
#    set_target_properties(mpi-mvapich-rt-offline PROPERTIES COMPILE_DEFINITIONS "${TLS_DEFINES};${LibUnwind_DEFINES};${Mvapich_DEFINES}") 
#
#    install(TARGETS mpi-mvapich-rt-offline
#	LIBRARY DESTINATION lib${LIB_SUFFIX}/openspeedshop
#    )
#
#    set_target_properties(mpi-mvapich-rt-offline PROPERTIES PREFIX "")
#
#    add_library(mpi-mvapich-offline STATIC
#        ${RT_SOURCES}
#	offline.c
#	wrappers.c
#	wrappers-fortran.c
#    )
#
#    target_include_directories(mpi-mvapich-offline PUBLIC
#	${CMAKE_CURRENT_SOURCE_DIR}
#	${CMAKE_CURRENT_BINARY_DIR}
#	${PROJECT_SOURCE_DIR}/libopenss-runtime
#	${PROJECT_BUILD_DIR}/libopenss-runtime
#	${PROJECT_SOURCE_DIR}/libopenss-runtime/offline
#	${LibMonitor_INCLUDE_DIRS}
#	${LibUnwind_INCLUDE_DIRS}
#	${Mvapich_INCLUDE_DIRS}
#    )
#
#
#    target_link_libraries(mpi-mvapich-offline
#	${CMAKE_DL_LIBS}
#	rt
#	pthread
#        -Wl,-whole-archive
#	openss-runtime-offline
#	openss-runtime-unwind
#        -Wl,-no-whole-archive
#	-Bdynamic
#	${LibUnwind_LIBRARY_SHARED}
#    )
#
#
#    #set_target_properties(mpi-mvapich-offline PROPERTIES VERSION 1.1.0)
#    set_target_properties(mpi-mvapich-offline PROPERTIES COMPILE_DEFINITIONS "${TLS_DEFINES};${LibUnwind_DEFINES};${Mvapich_DEFINES};OPENSS_STATIC") 
#    install(TARGETS mpi-mvapich-offline
#	ARCHIVE DESTINATION lib${LIB_SUFFIX}/openspeedshop
#    )
#
#
#endif()
#
#endif()
#
#file(COPY blobs.x DESTINATION .)
