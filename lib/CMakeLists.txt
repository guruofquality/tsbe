#
# Copyright 2011-2012 Josh Blum
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
# Check for support
########################################################################
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
find_package(NumaNuma)
find_package(AtomicOps)

if(NOT NUMANUMA_FOUND)
    message(FATAL_ERROR "no numanuma found!")
endif()

if(NOT Boost_FOUND)
    message(FATAL_ERROR "no boost found!")
endif()

if(NOT ATOMIC_OPS_FOUND)
    message(FATAL_ERROR "no atomic ops found!")
endif()

if(CMAKE_COMPILER_IS_GNUCXX)
    add_definitions(-Wall)
endif()

include_directories(${NUMANUMA_INCLUDE_DIRS})

include_directories(${ATOMIC_OPS_INCLUDE_DIRS})

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

########################################################################
# The sources
########################################################################
list(APPEND libtsbe_sources
    allocator.cpp
    buffer.cpp
    junction.cpp
    task.cpp
)

########################################################################
# Build the library
########################################################################
add_definitions(-DTSBE_DLL) #defined when building dynamic libraries
add_library(tsbe SHARED ${libtsbe_sources})
target_link_libraries(tsbe ${PTHREADS_LIBRARY} ${NUMANUMA_LIBRARIES} ${Boost_LIBRARIES})
set_target_properties(tsbe PROPERTIES DEFINE_SYMBOL "TSBE_DLL_EXPORTS")

########################################################################
# Install the library
########################################################################
install(TARGETS tsbe
    LIBRARY DESTINATION lib/${LIB_SUFFIX} COMPONENT "devel" # .so file
    ARCHIVE DESTINATION lib/${LIB_SUFFIX} COMPONENT "runtime" # .lib file
    RUNTIME DESTINATION bin COMPONENT "runtime" # .dll file
)
