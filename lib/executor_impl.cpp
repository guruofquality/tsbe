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

inline void ExecutorActor::send_topology_update(
    const std::vector<Element> &changed_block_set
){
    BOOST_FOREACH(const Element &block, changed_block_set)
    {
        BlockChangedMessage message_i;
        block->actor.Push(message_i, receiver.GetAddress());
        receiver.Wait();
    }
}

void ExecutorActor::handle_commit(
    const ExecutorCommitMessage &message,
    const Theron::Address from
){
    std::vector<Connection> new_flat_connections;
    std::vector<Element> changed_block_set;

    //step 1) resolve all connections in the topology
    {
        TopologyResolveConnectionsMessage message;
        message.result = &new_flat_connections;
        this->config.topology->actor.Push(message, receiver.GetAddress());
        receiver.Wait();
    }

    //step 2) determine new connections
    const std::vector<Connection> connections_to_add = vector_subtract(new_flat_connections, this->flat_connections);

    //step 3) add new inputs
    changed_block_set.clear();
    BOOST_FOREACH(const Connection &connection, connections_to_add)
    {
        BlockConnectMessage msg_i;
        msg_i.connection = connection;
        msg_i.action = BlockConnectMessage::SINK_CON;
        connection.sink.elem->actor.Push(msg_i, receiver.GetAddress());
        receiver.Wait();
        insert_unique(changed_block_set, connection.sink.elem);
    }
    send_topology_update(changed_block_set);

    //step 4) add new outputs
    changed_block_set.clear();
    BOOST_FOREACH(const Connection &connection, connections_to_add)
    {
        BlockConnectMessage msg_i;
        msg_i.connection = connection;
        msg_i.action = BlockConnectMessage::SRC_CON;
        connection.src.elem->actor.Push(msg_i, receiver.GetAddress());
        receiver.Wait();
        insert_unique(changed_block_set, connection.src.elem);
    }
    send_topology_update(changed_block_set);

    //step 5) determine old connections
    const std::vector<Connection> connections_to_remove = vector_subtract(this->flat_connections, new_flat_connections);

    //step 6) remove old outputs
    changed_block_set.clear();
    BOOST_FOREACH(const Connection &connection, connections_to_remove)
    {
        BlockConnectMessage msg_i;
        msg_i.connection = connection;
        msg_i.action = BlockConnectMessage::SRC_DIS;
        connection.src.elem->actor.Push(msg_i, receiver.GetAddress());
        receiver.Wait();
        insert_unique(changed_block_set, connection.src.elem);
    }
    send_topology_update(changed_block_set);

    //step 7) remove old inputs
    changed_block_set.clear();
    BOOST_FOREACH(const Connection &connection, connections_to_remove)
    {
        BlockConnectMessage msg_i;
        msg_i.connection = connection;
        msg_i.action = BlockConnectMessage::SINK_DIS;
        connection.sink.elem->actor.Push(msg_i, receiver.GetAddress());
        receiver.Wait();
        insert_unique(changed_block_set, connection.sink.elem);
    }
    send_topology_update(changed_block_set);

    //step 8) update the flat connections
    this->flat_connections = new_flat_connections;

    //step 9) get a unique list of blocks
    this->block_set.clear();
    this->block_set.reserve(this->flat_connections.size());
    BOOST_FOREACH(const Connection &connection, this->flat_connections)
    {
        insert_unique(this->block_set, connection.src.elem);
        insert_unique(this->block_set, connection.sink.elem);
    }

    if (from != Theron::Address()) this->Send(message, from); //ACK
}

void ExecutorActor::handle_post_msg(
    const ExecutorPostMessage &message,
    const Theron::Address from
){
    BOOST_FOREACH(const Element &block, this->block_set)
    {
        BlockPostMessage message_i;
        message_i.msg = message.msg;
        block->actor.Push(message_i, receiver.GetAddress());
        receiver.Wait();
    }

    if (from != Theron::Address()) this->Send(message, from); //ACK
}
