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

#ifndef INCLUDED_TSBE_TASK_HPP
#define INCLUDED_TSBE_TASK_HPP

#include <tsbe/config.hpp>
#include <tsbe/buffer.hpp>
#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>

namespace tsbe
{

//! The callback type for task processing
typedef boost::function<void(Task &)> TaskCallback;

//! Configuration struct for creating a task
struct TSBE_API TaskConfig
{
    TaskConfig(void);

    TaskCallback callback;
};

/*!
 * Task is simply a unit of processing.
 * A task contains a callback, which does the work,
 * and a set of API calls to access buffer IO.
 * The callback will be called when topology is active.
 */
struct TSBE_API Task : boost::shared_ptr<TaskImpl>
{
    //! Creates an empty task
    Task(void);

    //! Create a new task object from config
    Task(const TaskConfig &config);

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
     * Pop a buffer from the input queue.
     * The input queue contains filled buffers.
     * Throws if the queue was empty.
     */
    Buffer pop_input_buffer(const size_t index);

    /*!
     * Pop a buffer from the output queue.
     * The output queue contains empty buffers.
     * Throws if the queue was empty.
     */
    Buffer pop_output_buffer(const size_t index);

    //! Push a buffer onto the input queue
    void push_input_buffer(const size_t index, const Buffer &buff);

    //! Push a buffer onto the output queue
    void push_output_buffer(const size_t index, const Buffer &buff);

    /*!
     * Send a buffer to all subscribed outputs on this port.
     */
    void send_buffer(const size_t index, const Buffer &buff);

    //! Get the number of input ports (buffer consumption by task)
    size_t get_num_inputs(void);

    //! Get the number of output ports (buffer production by task)
    size_t get_num_outputs(void);

};

} //namespace tsbe

#endif /*INCLUDED_TSBE_TASK_HPP*/
