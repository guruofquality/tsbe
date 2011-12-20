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

find_path(
    NUMANUMA_INCLUDE_DIRS
    NAMES numanuma.h
    HINTS $ENV{NUMANUMA_DIR}/include
    PATHS /usr/local/include
          /usr/include
)

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")

    find_library(
        NUMA_LIBRARIES
        NAMES numa
        PATHS /usr/lib /usr/lib64
    )

    find_library(
        PTHREAD_LIBRARIES
        NAMES pthread
        PATHS /usr/lib /usr/lib64
    )

    find_library(
        RT_LIBRARIES
        NAMES rt
        PATHS /usr/lib /usr/lib64
    )

    set(NUMANUMA_LIBRARIES ${NUMA_LIBRARIES} ${PTHREAD_LIBRARIES} ${RT_LIBRARIES})

endif()

if(APPLE)

    find_library(
        PTHREAD_LIBRARIES
        NAMES pthread
        PATHS /usr/lib
    )

    set(NUMANUMA_LIBRARIES ${PTHREAD_LIBRARIES})

endif()

include(FindPackageHandleStandardArgs)

if(WIN32)
    unset(NUMANUMA_LIBRARIES)
    FIND_PACKAGE_HANDLE_STANDARD_ARGS(NUMANUMA DEFAULT_MSG NUMANUMA_INCLUDE_DIRS)
    mark_as_advanced(NUMANUMA_INCLUDE_DIRS)
else()
    FIND_PACKAGE_HANDLE_STANDARD_ARGS(NUMANUMA DEFAULT_MSG NUMANUMA_INCLUDE_DIRS NUMANUMA_LIBRARIES)
    mark_as_advanced(NUMANUMA_INCLUDE_DIRS NUMANUMA_LIBRARIES)
endif()
