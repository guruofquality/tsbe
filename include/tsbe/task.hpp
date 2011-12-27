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

#ifndef INCLUDED_TSBE_TASK_HPP
#define INCLUDED_TSBE_TASK_HPP

#include <tsbe/config.hpp>
#include <tsbe/queue.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>
#include <vector>

namespace tsbe{

//! Callback function typedef for a task
typedef boost::function<void(Task &)> TaskCB;

//! The configuration struct for making a new task
struct TSBE_API TaskConfig{

    //! Vector of input queues to pop
    std::vector<Queue> inputs;

    //! Vector of output queues to push
    std::vector<Queue> outputs;

    //! Callback to perform a round of processing
    TaskCB callback;
};

/*!
 * A task is a unit of processing.
 * Each task consists of a callback function and queues.
 * The function is called with this task.
 * The function should pop input buffers,
 * possibly grabbing memory from an allocator,
 * and finally pushing buffers to output queues.
 */
struct TSBE_API Task : boost::shared_ptr<TaskImpl>{

    //! Create a null/empty task
    Task(void);

    //! Create a new task from configuration
    Task(const TaskConfig &config);

    /*!
     * Pop a buffer from one of the input queues.
     * \param index a queue index 0 to num input queues - 1
     * \param timeout_ns timeout in nanoseconds (same rules as queue)
     * \return the popped buffer or null on timeout
     */
    Buffer pop_buffer(const size_t index, const long long timeout_ns);

    /*!
     * Push a buffer into one of the output queues.
     * \param index a queue index 0 to num output queues - 1
     * \param buff the filled buffer to push
     */
    void push_buffer(const size_t index, const Buffer &buff);

    /*!
     * One one round of processing.
     * Where a round ends when the callback returns.
     * If this task is in process with another thread,
     * this call will immediately return false.
     * \return true for success, false when blocked
     */
    bool run_one(void);

};

} //namespace tsbe

#endif /*INCLUDED_TSBE_TASK_HPP*/
