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

#include "task_impl.hpp"
#include <boost/foreach.hpp>

using namespace tsbe;

/***********************************************************************
 * task methods
 **********************************************************************/
TaskConfig::TaskConfig(void)
{
    //NOP
}

Task::Task(void)
{
    //NOP
}

Task::Task(const TaskConfig &config)
{
    this->reset(new TaskImpl(config));

    TaskActor::Parameters actor_params;
    actor_params.task = this->get();
    actor_params.task4cb = this;
    (*this)->actor = (*this)->framework.CreateActor<TaskActor>(actor_params);
}

const BitSet& Task::get_inputs_ready(void)
{
    return (*this)->inputs_ready;
}

const BitSet& Task::get_outputs_ready(void)
{
    return (*this)->outputs_ready;
}

Buffer Task::pop_input_buffer(const size_t index)
{
    //TODO throw bad index or empty
    std::queue<Buffer> &queue = (*this)->input_buffer_queues[index];
    Buffer buff = queue.front(); queue.pop();
    (*this)->inputs_ready.set(index, not queue.empty());
    return buff;
}

Buffer Task::pop_output_buffer(const size_t index)
{
    //TODO throw bad index or empty
    std::queue<Buffer> &queue = (*this)->output_buffer_queues[index];
    Buffer buff = queue.front(); queue.pop();
    (*this)->outputs_ready.set(index, not queue.empty());
    return buff;
}

void Task::push_input_buffer(const size_t index, const Buffer &buff)
{
    TaskBufferMessage message;
    message.buffer = buff;
    message.index = index;
    message.input = true;
    (*this)->actor.Push(message, Theron::Address::Null());
}

void Task::push_output_buffer(const size_t index, const Buffer &buff)
{
    TaskBufferMessage message;
    message.buffer = buff;
    message.index = index;
    message.input = false;
    (*this)->actor.Push(message, Theron::Address::Null());
}

void Task::send_buffer(const size_t index, const Buffer &buff)
{
    //TODO throw bad index
    BOOST_FOREACH(FlowEndpoint &ep, (*this)->outputs[index])
    {
        ep.task.push_input_buffer(ep.index, buff);
    }
}

size_t Task::get_num_inputs(void)
{
    return (*this)->inputs.size();
}

size_t Task::get_num_outputs(void)
{
    return (*this)->outputs.size();
}

void Task::set_affinity(const Affinity &affinity)
{
    (*this)->actor.Push(affinity, Theron::Address::Null());
}

Affinity Task::get_affinity(void)
{
    return (*this)->affinity;
}
