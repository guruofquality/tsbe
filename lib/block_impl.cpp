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

#include "block_impl.hpp"
#include "vec_utils.hpp"
#include <boost/foreach.hpp>

using namespace tsbe;

void BlockActor::handle_connect(
    const BlockConnectMessage &message,
    const Theron::Address from
){
    const Connection &connection = message.connection;

    //step 1) add or remove the new port connection
    switch (message.action)
    {
    case BlockConnectMessage::SRC_CON:
        vector_vector_add(interface->inputs, connection.src.index, connection.sink);
        break;

    case BlockConnectMessage::SINK_CON:
        vector_vector_add(interface->outputs, connection.sink.index, connection.src);
        break;

    case BlockConnectMessage::SRC_DIS:
        vector_vector_remove(interface->inputs, connection.src.index, connection.sink);
        break;

    case BlockConnectMessage::SINK_DIS:
        vector_vector_remove(interface->outputs, connection.sink.index, connection.src);
        break;
    }

    //step 2) resize other vectors to match new IO size
    interface->input_buffer_queues.resize(interface->inputs.size());
    interface->inputs_ready.resize(interface->inputs.size());

    interface->output_buffer_queues.resize(interface->outputs.size());
    interface->outputs_ready.resize(interface->outputs.size());

    this->Send(message, from); //ACK
}

void BlockActor::handle_downstream(
    const BlockDownstreamMessage &message,
    const Theron::Address from
){
    interface->input_buffer_queues[message.index].push(message.buffer);
    interface->inputs_ready.set(message.index, true);
    this->call_task();
}

void BlockActor::handle_return(
    const BlockReturnMessage &message,
    const Theron::Address from
){
    interface->output_buffer_queues[message.index].push(message.buffer);
    interface->outputs_ready.set(message.index, true);
    this->call_task();
}

void BlockActor::handle_any(
    const BlockAnyMessage &message,
    const Theron::Address from
){
    if (this->config.port_callback)
    {
        this->config.port_callback(message.index, message.msg);
    }
}

void BlockActor::call_task(void)
{
    if ((~interface->outputs_ready).none() and (~interface->inputs_ready).none())
    {
        this->config.task_callback(this->interface);
    }
}
