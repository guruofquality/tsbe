//
// Copyright 2011 Josh Blum
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
#include <boost/shared_ptr.hpp>
#include <boost/intrusive_ptr.hpp>

namespace tsbe{

static const int BUFFER_MODE_FLAG_RD = (1 << 0);
static const int BUFFER_MODE_FLAG_WR = (1 << 1);

//! Configuration struct for making a new buffer
struct BufferConfig{
    void *memory;
    size_t length;
    int mode_flags;
    int node_affinity;
};

/*!
 * Buffer object: memory, length, reference count...
 */
class TSBE_API Buffer{
public:
    //! Make a new buffer object
    static Buffer make(const BufferConfig &config);

    //! Null if the buffer is not allocated
    bool is_null(void) const;

    /*!
     * A reader of this buffer should mark it as done.
     * Internally, this decrements the reference count.
     * The num readers allows the caller to tell the buffer
     * how many extra things will also hold onto this buffer,
     * even after the current caller is done with the buffer.
     * \param new_readers extra buffer reference holders
     */
    void done_reading(const size_t new_readers = 0);

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

    /*!
     * Set the length in bytes of the buffer.
     * This is the number of bytes filled with useful data.
     * \param length the length in bytes
     */
    void set_length(const size_t length);

    struct Impl; boost::intrusive_ptr<Impl> _impl;
};

TSBE_API void intrusive_ptr_add_ref(Buffer::Impl *p);

TSBE_API void intrusive_ptr_release(Buffer::Impl *p);

} //namespace tsbe

#endif /*INCLUDED_TSBE_BUFFER_HPP*/
