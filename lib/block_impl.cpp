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

#include <tsbe_impl/block_impl.hpp>
#include <tsbe_impl/vec_utils.hpp>
#include <boost/foreach.hpp>
#include <boost/bind.hpp>

using namespace tsbe;

void BlockActor::handle_connect(
    const BlockConnectMessage &message,
    const Theron::Address from
){
    const Connection &connection = message.connection;

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

    this->Send(message, from); //ACK
}

void BlockActor::handle_input_msg(
    const BlockInputMessage &message,
    const Theron::Address from
){
    if (this->config.input_callback)
    {
        this->config.input_callback(this->task_iface, message.index, message.msg);
    }
}

void BlockActor::handle_output_msg(
    const BlockOutputMessage &message,
    const Theron::Address from
){
    if (this->config.output_callback)
    {
        this->config.output_callback(this->task_iface, message.index, message.msg);
    }
}

void BlockActor::handle_post_msg(
    const BlockPostMessage &message,
    const Theron::Address from
){
    if (this->config.block_callback)
    {
        this->config.block_callback(this->task_iface, message.msg);
    }
    if (from != Theron::Address()) this->Send(message, from); //ACK
}

void BlockActor::handle_changed(
    const BlockChangedMessage &message,
    const Theron::Address from
){
    if (this->config.changed_callback)
    {
        this->config.changed_callback(this->task_iface);
    }
    this->Send(message, from); //ACK
}
