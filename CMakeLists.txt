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
project(tsbe CXX)
enable_testing()

########################################################################
# Local Include Dir
########################################################################
include_directories(${CMAKE_SOURCE_DIR}/include)

install(FILES
    include/tsbe/config.hpp
    include/tsbe/buffer.hpp
    include/tsbe/queue.hpp
    DESTINATION include/tsbe
    COMPONENT "devel"
)

########################################################################
# Test it
########################################################################
add_subdirectory(lib)
