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

#include <tsbe/task.hpp>
#include "tsbe_impl.hpp"
#include <boost/foreach.hpp>
#include <vector>
#include <queue>

using namespace tsbe;

Task::Task(void)
{
    //NOP
}

Task::Task(const TaskConfig &config)
{
    this->reset(new TaskImpl());
    (*this)->config = config;

    TaskActor::Parameters actor_params;
    actor_params.task = *this;
    (*this)->actor = (*this)->framework.CreateActor<TaskActor>(actor_params);
}


bool Task::has_input_buffer(const size_t index)
{
    return not (*this)->input_buffer_queues[index].empty();
}

Buffer Task::pop_input_buffer(const size_t index)
{
    Buffer buff = (*this)->input_buffer_queues[index].front();
    (*this)->input_buffer_queues[index].pop();
    return buff;
}

void Task::push_output_buffer(const Buffer &buff, const size_t index)
{
    BOOST_FOREACH(TaskDestination &destination, (*this)->output_destinations[index])
    {
        TaskBufferMessage message;
        message.buffer = buff;
        message.index = destination.index;
        Theron::Address from_addr = (*this)->actor.GetAddress();
        destination.actor.Push(message, from_addr);
    }
}
