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

#ifndef INCLUDED_TSBE_JUNCTION_HPP
#define INCLUDED_TSBE_JUNCTION_HPP

#include <tsbe/config.hpp>
#include <tsbe/buffer.hpp>
#include <boost/shared_ptr.hpp>

namespace tsbe{

//! The configuration struct for making a new junction
struct TSBE_API JunctionConfig{
    size_t num_outputs;
};

/*!
 * Junction structure:
 * Data goes in, data goes out. You can't explain that.
 *
 * A junction (think plumbing) is a queue-like data structure.
 * Buffers are committed into the junction and duplicated to each output.
 * In addition to buffers, contiguous memory that crosses buffers,
 * can be transfered from a junction output to another junction.
 */
struct TSBE_API Junction : boost::shared_ptr<JunctionImpl>{

    //! Make a null junction
    Junction(void);

    //! Make a new junction for buffers
    Junction(const JunctionConfig &config);

    /*!
     * Adopt a buffer makes this junction the owner.
     * When the buffer's reference count goes to zero,
     * it will automatically be pushed into this junction.
     * Will thrown if the buffer already has an owner.
     */
    void adopt(const Buffer &buff) const;

    /*!
     * Disown the buffer, it is now free to delete.
     * Will throw if this junction is not the owner.
     */
    void disown(const Buffer &buff) const;

    /*!
     * Push a buffer into the back of the junction.
     * This operation does not block and is always successful.
     * The junction internals will be resized if necessary.
     */
    void push(const Buffer &buff);

    /*!
     * Get a buffer from the front of the junction.
     * Waits for the num bytes to become available.
     * Buffer will be non-null on successful call.
     * Rules for num_bytes:
     * - bytes = 0, wait for any number of bytes
     * - bytes > 0, wait for exactly this many bytes
     * Rules for timeout:
     * - timeout > 0, actual timeout operation
     * - timeout = 0, immediate return on empty
     * - timeout < 0, block forever (not recommended)
     * \param index the output index of this junction
     * \param num_bytes the number of bytes to wait for
     * \param timeout_ns the timeout in nanoseconds
     * \return the buffer or null on timeout
     */
    Buffer front(const size_t index, const size_t num_bytes, const long long timeout_ns);

    /*!
     * Pop/remove num bytes from the front of the junction.
     * At least num bytes should be pop-able from this output.
     * This can be guaranteed by using the front method.
     * Otherwise the pop() routine will throw an exception.
     * \param index the output index of this junction
     * \param num_bytes the number of bytes to pop
     */
    void pop(const size_t index, const size_t num_bytes);

};

} //namespace tsbe

#endif /*INCLUDED_TSBE_JUNCTION_HPP*/
