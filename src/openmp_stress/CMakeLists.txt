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

set(OPENMP_STRESS_SOURCES
	stress_omp.f
    )

message(STATUS "BUILD_COMPILER_NAME=" ${BUILD_COMPILER_NAME} )
message(STATUS "OpenMP_FLAG_DETECTED=" ${OpenMP_FLAG_DETECTED} )
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

# OpenMP executable section -------------------------------------------

add_executable(openmp-openmp_stress
    ${OPENMP_STRESS_SOURCES}
)

target_include_directories(openmp-openmp_stress PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${OpenMP_INCLUDE_DIRS}
)

target_link_libraries(openmp-openmp_stress
    ${CMAKE_DL_LIBS}
    ${OPENMP_LIBS}
    m
)

set_target_properties(openmp-openmp_stress PROPERTIES COMPILE_DEFINITIONS "${OpenMPI_DEFINES};USE_MPI=1")
set_target_properties(openmp-openmp_stress PROPERTIES OUTPUT_NAME "openmp-openmp_stress-${BUILD_COMPILER_NAME}")

install(TARGETS openmp-openmp_stress
        DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
)
