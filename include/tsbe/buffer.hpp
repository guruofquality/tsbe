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

#ifndef INCLUDED_TSBE_BUFFER_HPP
#define INCLUDED_TSBE_BUFFER_HPP

#include <tsbe/config.hpp>
#include <boost/function.hpp>
#include <boost/intrusive_ptr.hpp>

namespace tsbe{

static const int BUFFER_MODE_FLAG_RD = (1 << 0);
static const int BUFFER_MODE_FLAG_WR = (1 << 1);

typedef boost::function<void(void)> BufferCB;

//! Configuration struct for making a new buffer
struct TSBE_API BufferConfig{

    //! pointer to the base of the memory
    void *memory;

    //! length of the memory in bytes
    size_t length;

    //! mode flags: readable and or writable
    int mode_flags;

    //! memory node affinity, -1 for no association
    int node_affinity;

    //! callback when buffer deconstruction (optional)
    BufferCB callback;
};

/*!
 * Buffer object: memory, length, reference count...
 */
struct TSBE_API Buffer : boost::intrusive_ptr<BufferImpl>{

    //! Make a null/empty buffer
    Buffer(void);

    //! Make a new buffer object
    Buffer(const BufferConfig &config);

    /*!
     * Get the RW mode of the buffer.
     * Note: A writable buffer will not have WR set when it has multiple readers.
     * \return the RW mode flags
     */
    int get_mode_flags(void) const;

    /*!
     * Get the memory node affinity of this buffer.
     * \return the node affinity or -1 for not applicable
     */
    int get_node_affinity(void) const;

    //! Get the memory address of the buffer
    void *get_memory(void) const;

    //! Get the length in bytes of the buffer
    size_t get_length(void) const;

};

} //namespace tsbe

#endif /*INCLUDED_TSBE_BUFFER_HPP*/
