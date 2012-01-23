//
// Copyright 2011-2012 Josh Blum
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

#ifndef INCLUDED_TSBE_BUFFER_TOKEN_HPP
#define INCLUDED_TSBE_BUFFER_TOKEN_HPP

#include <tsbe/config.hpp>
#include <boost/function.hpp>
#include <boost/intrusive_ptr.hpp>

namespace tsbe{

struct BufferToken;

typedef boost::function<void(BufferToken)> BufferTokenConfigCB;

struct TSBE_API BufferTokenConfig
{
    //! pointer to the beginning of a chunk of memory
    void *memory;

    //! the length of a chunk of memory in bytes
    size_t length;

    //! optional index for sorting tokens
    size_t index;
};

/*!
 * A buffer token is a reference counted object.
 * When the reference count goes to zero, the token is returned to its parent.
 * Parent refers to a buffer pool.... TODO
 */
struct TSBE_API BufferToken:
    boost::intrusive_ptr<BufferTokenImpl>
{
    //! Create an empty buffer token
    BufferToken(void);

    //! Create a new buffer token from config
    BufferToken(const BufferTokenConfig &config);

    //! Get the pointer to the memory
    void *get_memory(void) const;

    //! Get the length of the memory in bytes
    size_t get_length(void) const;

    //! Get the index set for this token
    size_t get_index(void) const;
};

} //namespace tsbe

#endif /*INCLUDED_TSBE_BUFFER_TOKEN_HPP*/
