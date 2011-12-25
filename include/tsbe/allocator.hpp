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

#ifndef INCLUDED_TSBE_ALLOCATOR_HPP
#define INCLUDED_TSBE_ALLOCATOR_HPP

#include <tsbe/config.hpp>
#include <tsbe/buffer.hpp>
#include <boost/shared_ptr.hpp>

namespace tsbe{

struct AllocatorConfig{

    //! length of each buffer in bytes
    size_t length;

    //! the node affinity of the memory, -1 for don't care
    int node_affinity;

    //! the number of buffers to allocate on init
    size_t default_num_buffers;

    //! the maximum number of buffers to allocate
    size_t maximum_num_buffers;
};


/*!
 * The allocator object provides a source for empty buffers.
 * The user can set some basic policy rules for the allocator.
 *
 * Every buffer's memory is allocated to be page aligned.
 * TODO: add a page alignment attribute to the config struct
 *
 * Allocator is nothing more than a queue, and some logic.
 * For anything more complicated than this allocator,
 * the user should manually use a queue and allocate buffers.
 * Example: buffers that reside in a specific portion of memory
 */
struct TSBE_API Allocator : public boost::shared_ptr<AllocatorImpl>{

    //! Make a null/empty allocator
    Allocator(void);

    //! Make a new allocator object
    Allocator(const AllocatorConfig &config);

    /*!
     * Get a buffer from the allocator.
     * The allocator will return the first buffer available.
     * If no buffer is available, one will be allocated.
     * If the maximum number of buffers is already allocated,
     * this call will block and timeout waiting for a buffer.
     * Rules for timeout:
     * - timeout > 0, actual timeout operation
     * - timeout = 0, immediate return on empty
     * - timeout < 0, block forever (not recommended)
     * \param timeout_ns the timeout in nanoseconds
     * \return the buffer or null on timeout
     */
    Buffer get_buffer(const long long timeout_ns);

};

} //namespace tsbe

#endif /*INCLUDED_TSBE_ALLOCATOR_HPP*/
