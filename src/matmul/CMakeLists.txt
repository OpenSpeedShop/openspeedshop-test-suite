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

project (matmul Fortran)

set(MATMUL_SOURCES
	matmul.f90
)

message(STATUS "BUILD_COMPILER_NAME=" ${BUILD_COMPILER_NAME} )


add_executable(sequential-matmul
    ${MATMUL_SOURCES}
)

target_include_directories(sequential-matmul PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(sequential-matmul
    blas
    lapack
    ${CMAKE_DL_LIBS}
)

set_target_properties(sequential-matmul PROPERTIES OUTPUT_NAME "sequential-matmul-${BUILD_COMPILER_NAME}")

install(TARGETS sequential-matmul
    DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
)

