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

#ifndef INCLUDED_TSBE_QUEUE_HPP
#define INCLUDED_TSBE_QUEUE_HPP

#include <tsbe/config.hpp>
#include <tsbe/buffer.hpp>
#include <boost/shared_ptr.hpp>

namespace tsbe{

/*!
 * Queue structure for pushing and popping buffers.
 */
struct TSBE_API Queue : public boost::shared_ptr<QueueImpl>{

    //! Make a null queue
    Queue(void);

    //! Make a new queue for buffers
    Queue(const bool);

    /*!
     * Adopt a buffer makes this queue the owner.
     * When the buffer's reference count goes to zero,
     * it will automatically be pushed into this queue.
     * Will thrown if the buffer already has an owner.
     */
    void adopt(const Buffer &buff) const;

    /*!
     * Disown the buffer, it is now free to delete.
     * Will throw if this queue is not the owner.
     */
    void disown(const Buffer &buff) const;

    /*!
     * Push a buffer into the back of the queue.
     * This operation does not block,
     * and always successfully pushes.
     * The queue internals will be resized if necessary.
     */
    void push(const Buffer &buff);

    /*!
     * Pop a buffer from the front of the queue.
     * Waits for a buffer to become available.
     * Buffer will be non-null on successful pop.
     * Rules for timeout:
     * - timeout > 0, actual timeout operation
     * - timeout = 0, immediate return on empty
     * - timeout < 0, block forever (not recommended)
     */
    void pop(Buffer &buff, const long long timeout_ns);
};

} //namespace tsbe

#endif /*INCLUDED_TSBE_QUEUE_HPP*/
