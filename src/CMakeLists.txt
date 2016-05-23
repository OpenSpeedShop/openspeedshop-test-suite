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

# sequential only
add_subdirectory(loopcheck)
#add_subdirectory(matmul)
add_subdirectory(threads)
# openmp only
add_subdirectory(openmp_stress)
# mpi only
add_subdirectory(nbody)
add_subdirectory(sweep3d)
add_subdirectory(mpi_wave)
add_subdirectory(IOR)
# sequential and mpi
add_subdirectory(lulesh)
# hybrid(openmp/mpi) and mpi only
add_subdirectory(stream)
if(NOT BUILD_COMPILER_NAME MATCHES pgi)
add_subdirectory(lulesh2.0.3)
endif()



