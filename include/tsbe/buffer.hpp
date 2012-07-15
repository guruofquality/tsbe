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
#include <boost/function.hpp>

namespace tsbe
{

//! The callback function type when buffers dereference
typedef boost::function<void(Buffer &)> BufferDeleter;

struct TSBE_API BufferConfig
{
    //! Default constructor zeros out buffer config
    BufferConfig(void);

    //! pointer to the memory start
    void *memory;

    //! length of the memory in bytes
    size_t length;

    //! memory affinity - meta information
    Affinity affinity;

    //! deleter callback, may be used to free
    BufferDeleter deleter;

    //! token object, called if set under deref condition
    boost::weak_ptr<BufferDeleter> token;
};

/*!
 * Buffer is a reference counted smart pointer handler of memory.
 *
 * Default allocator:
 * To use the default system allocator, set the memory to NULL,
 * and set length and affinity to the desired values.
 * The deleter will be automaically configured by the allocator.
 *
 * Custom allocator:
 * Set all config struct members. Its all you!
 *
 * Token usage:
 * When a token is set, the buffer will not cleanup and call the deleter.
 * Rather, the bound function in the token will be called with the buffer.
 * A parent object should hold the shared pointer to the token.
 * When the parent object deconstructs, the weak pointer will die,
 * and the normal buffer cleanup/freeing/deconstruction will happen.
 *
 * Length and memory usage:
 * Length and memory are intentionally references
 * so that they can be modified (carefully).
 * The user may do this to pass a subset of the memory.
 * Using the token causes a reset of these values.
 */
struct TSBE_API Buffer : boost::intrusive_ptr<BufferImpl>
{
    //! Default constructor, zeros things
    Buffer(void);

    /*!
     * Create a new buffer.
     * The config object represents a chunk of memory,
     * or instructions for the default allocator.
     */
    Buffer(const BufferConfig &config);

    /*!
     * Get a pointer to the start of the underlying memory
     */
    void *get_actual_memory(void) const;

    /*!
     * Get the length of the underlying memory in bytes
     */
    size_t get_actual_length(void) const;

    /*!
     * Get a pointer to the start of memory.
     * This value defaults to the get_actual_memory(),
     * and can be modified by the user for an offset.
     * When the token is invoked, this value will be reset.
     */
    void *&get_memory(void);

    /*!
     * Get the length of the memory in bytes.
     * This value defaults to the get_actual_length(),
     * and can be modified by the user when partially filled.
     * When the token is invoked, this value will be reset.
     */
    size_t &get_length(void);

    //! Get the affinity of the memory
    Affinity get_affinity(void) const;
};

} //namespace tsbe

#endif /*INCLUDED_TSBE_BUFFER_HPP*/
