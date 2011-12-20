#
# Copyright 2011 Josh Blum
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

########################################################################
# Project setup
########################################################################
cmake_minimum_required(VERSION 2.6)
project(tsbe C CXX)
enable_testing()

########################################################################
# Local Include Dir
########################################################################
include_directories(${CMAKE_SOURCE_DIR}/include)

install(FILES
    include/tsbe/api.h
    include/tsbe/types.h
    include/tsbe/buffer.h
    include/tsbe/queue.h
    DESTINATION include/tsbe
    COMPONENT "devel"
)

########################################################################
# Test it
########################################################################
add_subdirectory(lib)
