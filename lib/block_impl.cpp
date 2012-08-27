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
#include <boost/bind.hpp>

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
        vector_vector_add(task_iface->outputs, connection.src.index, connection.sink);
        break;

    case BlockConnectMessage::SINK_CON:
        vector_vector_add(task_iface->inputs, connection.sink.index, connection.src);
        break;

    case BlockConnectMessage::SRC_DIS:
        vector_vector_remove(task_iface->outputs, connection.src.index, connection.sink);
        break;

    case BlockConnectMessage::SINK_DIS:
        vector_vector_remove(task_iface->inputs, connection.sink.index, connection.src);
        break;
    }

    //step 2) resize other vectors to match new IO size
    task_iface->input_buffer_queues.resize(task_iface->inputs.size());
    task_iface->inputs_ready.resize(task_iface->inputs.size());

    task_iface->output_buffer_queues.resize(task_iface->outputs.size());
    task_iface->outputs_ready.resize(task_iface->outputs.size());

    this->Send(message, from); //ACK
}

void BlockActor::handle_downstream(
    const BlockDownstreamMessage &message,
    const Theron::Address from
){
    task_iface->input_buffer_queues[message.index].push(message.buffer);
    task_iface->inputs_ready.set(message.index, true);
    this->config.task_callback(this->task_iface);
}

void BlockActor::handle_return(
    const BlockReturnMessage &message,
    const Theron::Address from
){
    task_iface->output_buffer_queues[message.index].push(message.buffer);
    task_iface->outputs_ready.set(message.index, true);
    this->config.task_callback(this->task_iface);
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

void BlockActor::post_return_buffer(const size_t index, Buffer &buffer)
{
    BlockReturnMessage message;
    message.buffer = buffer;
    message.index = index;
    this->Send(message, this->GetAddress()); //send to self
}

void BlockActor::handle_allocator(
    const BlockAllocatorMessage &message,
    const Theron::Address from
){
    //make a new token
    BufferToken tok(new BufferDeleter(boost::bind(
        &BlockActor::post_return_buffer, this, message.index, _1)));

    //save the token
    task_iface->output_buffer_tokens.resize(task_iface->outputs.size());
    task_iface->output_buffer_tokens[message.index] = tok;

    //actually allocate
    message.alloc(tok);
}

void BlockActor::handle_update(
    const BlockUpdateMessage &message,
    const Theron::Address from
){
    if (this->config.update_callback)
    {
        this->config.update_callback(this->task_iface, message.state);
    }
}
