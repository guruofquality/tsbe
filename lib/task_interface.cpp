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

void TaskInterface::post_downstream(const size_t index, const Wax &msg) const
{
    //TODO throw bad index
    BlockInputMessage message;
    message.msg = msg;
    BOOST_FOREACH(Port &port, (*this)->outputs[index])
    {
        message.index = port.index;
        port.elem->framework.Send(message, Theron::Address(), port.elem->actor->GetAddress());
    }
}

void TaskInterface::post_upstream(const size_t index, const Wax &msg) const
{
    //TODO throw bad index
    BlockOutputMessage message;
    message.msg = msg;
    BOOST_FOREACH(Port &port, (*this)->inputs[index])
    {
        message.index = port.index;
        port.elem->framework.Send(message, Theron::Address(), port.elem->actor->GetAddress());
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
