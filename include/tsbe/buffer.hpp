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
#include <tsbe/token.hpp>
#include <boost/asio/buffer.hpp>

namespace tsbe{

//! Wrapper for a constant buffer
struct TSBE_API ConstBuffer:
    boost::asio::const_buffer
{
    //! Create a null const buffer
    ConstBuffer(void);

    //! Create a const buffer from pointer and length in bytes
    ConstBuffer(const void *mem, const size_t len);

    //! Get the size of the const buffer in bytes
    size_t size(void) const;

    //! Cast the memory pointer to a pointer of type
    template <typename Type>
    const Type *cast(void) const;
};

//! Wrapper for a mutable buffer
struct TSBE_API MutableBuffer:
    boost::asio::mutable_buffer
{
    //! Create a null mutable buffer
    MutableBuffer(void);

    //! Create a mutable buffer from pointer and length in bytes
    MutableBuffer(void *mem, const size_t len);

    //! Get the size of the const buffer in bytes
    size_t size(void) const;

    //! Cast the memory pointer to a pointer of type
    template <typename Type>
    Type *cast(void) const;
};

/*!
 * A lightweight buffer object.
 * Has a pointer to memory, length, and token.
 * When the token reference goes to zero,
 * the callback is executed, presumably,
 * the buffer is restored to a source pool.
 */
struct TSBE_API BufferToken
{
    //! The buffer containing mutable memory
    MutableBuffer buff;

    //! The token with bound callback
    Token token;

    //!TODO a way to claim this buffer for reading
    //!Probably make this part of being a token
};

} //namespace tsbe

#include <tsbe/detail/buffer.hpp>

#endif /*INCLUDED_TSBE_BUFFER_HPP*/
