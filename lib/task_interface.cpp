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

#include "task_interface_impl.hpp"
#include "block_impl.hpp"
#include <boost/foreach.hpp>

using namespace tsbe;

TaskInterface::TaskInterface(void)
{
    //NOP
}

const BitSet& TaskInterface::get_inputs_ready(void) const
{
    return (*this)->inputs_ready;
}

const BitSet& TaskInterface::get_outputs_ready(void) const
{
    return (*this)->outputs_ready;
}

Buffer &TaskInterface::get_input_buffer(const size_t index) const
{
    //TODO throw bad index or empty
    return (*this)->input_buffer_queues[index].front();
}

Buffer &TaskInterface::get_output_buffer(const size_t index) const
{
    //TODO throw bad index or empty
    return (*this)->output_buffer_queues[index].front();
}

void TaskInterface::pop_input_buffer(const size_t index) const
{
    //TODO throw bad index or empty
    (*this)->input_buffer_queues[index].pop();
    (*this)->inputs_ready.set(index, not (*this)->input_buffer_queues[index].empty());
}

void TaskInterface::pop_output_buffer(const size_t index) const
{
    //TODO throw bad index or empty
    (*this)->output_buffer_queues[index].pop();
    (*this)->outputs_ready.set(index, not (*this)->output_buffer_queues[index].empty());
}

void TaskInterface::post_downstream(const size_t index, const Buffer &buffer) const
{
    //TODO throw bad index
    BlockDownstreamMessage message;
    message.buffer = buffer;
    BOOST_FOREACH(Port &port, (*this)->outputs[index])
    {
        message.index = port.index;
        port.elem->actor.Push(message, Theron::Address());
    }
}

void TaskInterface::post_downstream(const size_t index, const boost::any &msg) const
{
    //TODO throw bad index
    BlockAnyMessage message;
    message.msg = msg;
    BOOST_FOREACH(Port &port, (*this)->outputs[index])
    {
        message.index = port.index;
        port.elem->actor.Push(message, Theron::Address());
    }
}

size_t TaskInterface::get_num_inputs(void) const
{
    return (*this)->inputs.size();
}

size_t TaskInterface::get_num_outputs(void) const
{
    return (*this)->outputs.size();
}
