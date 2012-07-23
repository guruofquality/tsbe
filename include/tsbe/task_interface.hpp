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

#ifndef INCLUDED_TSBE_TASK_INTERFACE_HPP
#define INCLUDED_TSBE_TASK_INTERFACE_HPP

#include <tsbe/config.hpp>
#include <boost/any.hpp>

namespace tsbe
{

/*!
 * A task interface is passed into the task callback of a block.
 * Task interfaces are created by the system and user-contructed.
 */
struct TSBE_API TaskInterface : boost::shared_ptr<TaskInterfaceImpl>
{

    //! Creates a null task interface
    TaskInterface(void);

    //! Get the number of input ports (buffer consumption by task)
    size_t get_num_inputs(void);

    //! Get the number of output ports (buffer production by task)
    size_t get_num_outputs(void);

    /*!
     * Get a bitset representing ready inputs.
     * The bit set will be num inputs wide.
     * If bitset[index] == 1, index has a buffer.
     */
    const BitSet& get_inputs_ready(void);

    /*!
     * Get a bitset representing ready outputs.
     * The bit set will be num outputs wide.
     * If bitset[index] == 1, index has a buffer.
     */
    const BitSet& get_outputs_ready(void);

    /*!
     * Get access to the buffer in the front of the queue.
     * The input queue contains filled buffers.
     * Buffer is NULL if the queue was empty.
     */
    Buffer &get_input_buffer(const size_t index);

    /*!
     * Get access to the buffer in the front of the queue.
     * The output queue contains empty buffers.
     * Buffer is NULL if the queue was empty.
     */
    Buffer &get_output_buffer(const size_t index);

    /*!
     * Remove a buffer from the front of the input queue.
     * The input queue contains filled buffers.
     */
    void pop_input_buffer(const size_t index);

    /*!
     * Pop a buffer from the output queue.
     * The output queue contains empty buffers.
     */
    void pop_output_buffer(const size_t index);

    /*!
     * Send a buffer to all subscribed outputs on this port.
     * \param index the output port index
     */
    void post_downstream(const size_t index, const Buffer &buffer);

    /*!
     * Send a message to all subscribed outputs on this port.
     * \param index the output port index
     */
    void post_downstream(const size_t index, const boost::any &msg);

};

} //namespace tsbe

#endif /*INCLUDED_TSBE_TASK_INTERFACE_HPP*/
