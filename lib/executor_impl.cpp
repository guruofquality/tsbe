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

#include "executor_impl.hpp"
#include "topology_impl.hpp"
#include "block_impl.hpp"
#include "vec_utils.hpp"
#include <boost/foreach.hpp>

using namespace tsbe;

void ExecutorActor::handle_update(
    const ExecutorUpdateMessage &message,
    const Theron::Address from
){
    std::vector<Connection> new_flat_connections;

    //step 1) resolve all connections in the topology
    {
        Theron::Receiver receiver;
        TopologyResolveConnectionsMessage message;
        message.result = &new_flat_connections;
        this->config.topology->actor.Push(message, receiver.GetAddress());
        receiver.Wait();
    }

    //step 2) remove old connections
    const std::vector<Connection> connections_to_remove = vector_subtract(this->flat_connections, new_flat_connections);
    BOOST_FOREACH(const Connection &connection, connections_to_remove)
    {
        Theron::Receiver receiver;
        BlockConnectMessage msg_i;
        msg_i.connection = connection;

        msg_i.action = BlockConnectMessage::SRC_DIS;
        connection.src.elem->actor.Push(msg_i, receiver.GetAddress());
        receiver.Wait();

        msg_i.action = BlockConnectMessage::SINK_DIS;
        connection.sink.elem->actor.Push(msg_i, receiver.GetAddress());
        receiver.Wait();
    }

    //step 3) create new connections
    const std::vector<Connection> connections_to_add = vector_subtract(new_flat_connections, this->flat_connections);
    BOOST_FOREACH(const Connection &connection, connections_to_add)
    {
        Theron::Receiver receiver;
        BlockConnectMessage msg_i;
        msg_i.connection = connection;

        msg_i.action = BlockConnectMessage::SRC_CON;
        connection.src.elem->actor.Push(msg_i, receiver.GetAddress());
        receiver.Wait();

        msg_i.action = BlockConnectMessage::SINK_CON;
        connection.sink.elem->actor.Push(msg_i, receiver.GetAddress());
        receiver.Wait();
    }

    //step 4) update the flat connections
    this->flat_connections = new_flat_connections;

    //step 5) get a unique list of blocks
    std::vector<Element> block_set;
    block_set.reserve(this->flat_connections.size());
    BOOST_FOREACH(const Connection &connection, this->flat_connections)
    {
        remove_one(block_set, connection.src.elem);
        block_set.push_back(connection.src.elem);
        remove_one(block_set, connection.sink.elem);
        block_set.push_back(connection.sink.elem);
    }

    //step 6) pass the update message to blocks
    BOOST_FOREACH(const Element &block, block_set)
    {
        BlockUpdateMessage msg;
        msg.state = message.state;
        block->actor.Push(msg, Theron::Address());
    }

    this->Send(message, from); //ACK
}
