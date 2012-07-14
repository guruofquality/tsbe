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

#include "tsbe_impl.hpp"
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
    actor_params.task = *this;
    (*this)->actor = (*this)->framework.CreateActor<TaskActor>(actor_params);
}


bool Task::has_input_buffer(const size_t index)
{
    //TODO throw bad index
    return not (*this)->input_buffer_queues[index].empty();
}

bool Task::has_output_buffer(const size_t index)
{
    //TODO throw bad index
    return not (*this)->output_buffer_queues[index].empty();
}

Buffer Task::pop_input_buffer(const size_t index)
{
    //TODO throw bad index or empty
    Buffer buff = (*this)->input_buffer_queues[index].front();
    (*this)->input_buffer_queues[index].pop();
    return buff;
}

Buffer Task::pop_output_buffer(const size_t index)
{
    //TODO throw bad index or empty
    Buffer buff = (*this)->output_buffer_queues[index].front();
    (*this)->output_buffer_queues[index].pop();
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
    BOOST_FOREACH(Endpoint &ep, (*this)->outputs[index])
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
