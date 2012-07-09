//
// Copyright 2012 Josh Blum
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef INCLUDED_TSBE_BUFFER_HPP
#define INCLUDED_TSBE_BUFFER_HPP

#include <tsbe/config.hpp>
#include <tsbe/deleter.hpp>

namespace tsbe
{

/*!
 * Buffer is a simple object with a pointer and length.
 * The expected use of deleter is to free the memory.
 */
struct TSBE_API Buffer
{
    //! Default constructor, zeros things
    Buffer(void);

    //! pointer to the memory start
    void *memory;

    //! length of the memory in bytes
    size_t length;

    //! memory affinity - meta information
    size_t affinity;

    //! deleter object, may be used to free
    Deleter deleter;
};

} //namespace tsbe

#endif /*INCLUDED_TSBE_BUFFER_HPP*/
