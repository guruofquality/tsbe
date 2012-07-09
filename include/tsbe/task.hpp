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

    size_t num_threads;
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

    bool has_input_buffer(const size_t index);

    Buffer pop_input_buffer(const size_t index);

    void push_output_buffer(const Buffer &buff, const size_t index);

    size_t get_num_inputs(void);
    size_t get_num_outputs(void);
};

} //namespace tsbe

#endif /*INCLUDED_TSBE_TASK_HPP*/
