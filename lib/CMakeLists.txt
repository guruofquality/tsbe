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
# Compiler specific setup
########################################################################
if(CMAKE_COMPILER_IS_GNUCXX AND NOT WIN32)
    #http://gcc.gnu.org/wiki/Visibility
    add_definitions(-fvisibility=hidden)
    add_definitions(-fvisibility-inlines-hidden)
endif()

########################################################################
# Check for support
########################################################################
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

if(NOT Boost_FOUND)
    message(FATAL_ERROR "no boost found!")
endif()

if(CMAKE_COMPILER_IS_GNUCXX)
    add_definitions(-Wall)
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

########################################################################
# The sources
########################################################################
list(APPEND libtsbe_sources
    block.cpp
    block_impl.cpp
    buffer.cpp
    connection.cpp
    executor.cpp
    executor_impl.cpp
    port.cpp
    task_interface.cpp
    topology.cpp
    topology_impl.cpp
)

list(APPEND libtsbe_libs
    ${Boost_LIBRARIES}
    ${NUMANUMA_LIBRARIES}
)

########################################################################
# Build the shared library
########################################################################
add_library(tsbe SHARED ${libtsbe_sources})
target_link_libraries(tsbe ${libtsbe_libs})
set_target_properties(tsbe PROPERTIES DEFINE_SYMBOL "TSBE_DLL_EXPORTS")
set_target_properties(tsbe PROPERTIES COMPILE_DEFINITIONS "TSBE_DLL=1")

########################################################################
# Build the static library
########################################################################
add_library(tsbe_static STATIC ${libtsbe_sources})
target_link_libraries(tsbe_static ${libtsbe_libs})

########################################################################
# Install the library
########################################################################
install(TARGETS tsbe tsbe_static
    LIBRARY DESTINATION lib/${LIB_SUFFIX} COMPONENT "devel" # .so file
    ARCHIVE DESTINATION lib/${LIB_SUFFIX} COMPONENT "runtime" # .lib file
    RUNTIME DESTINATION bin COMPONENT "runtime" # .dll file
)
