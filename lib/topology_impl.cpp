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

#include "topology_impl.hpp"
#include "vec_utils.hpp"
#include <boost/foreach.hpp>

using namespace tsbe;

void TopologyActor::handle_connect(
    const TopologyConnectMessage &message,
    const Theron::Address from
){
    switch(message.action)
    {
    case TopologyConnectMessage::CONNECT:
        this->connections.push_back(message.connection);
        break;

    case TopologyConnectMessage::DISCONNECT:
        if (not remove_one(this->connections, message.connection))
        {
            //TODO pass error, dont throw here
            throw std::runtime_error("Topology::disconnect could not find connection");
        }
        break;

    case TopologyConnectMessage::ADD:
        this->topologies.push_back(message.topology);
        break;

    case TopologyConnectMessage::REMOVE:
        if (not remove_one(this->topologies, message.topology))
        {
            //TODO pass error, dont throw here
            throw std::runtime_error("Topology::remove_topology could not find topology");
        }
        break;
    }

    this->Send(message, from); //ACK
}

void TopologyActor::handle_resolve_ports(
    const TopologyResolvePortsMessage &message,
    const Theron::Address from
){
    std::vector<Port> &ports = *message.result;
    ports.clear();

    //check for weird conditions
    if (not message.port.elem)
    {
        throw std::runtime_error("resolve ports total inception error!");
    }

    //check if this is a normal block
    if (message.port.elem->is_block())
    {
        ports.push_back(message.port);
    }

    //otherwise, recurse into the sub topology
    else BOOST_FOREACH(Connection &connection, this->connections)
    {
        bool found = false;
        TopologyResolvePortsMessage message_i;

        if (
            message.action == TopologyResolvePortsMessage::SRC and
            not connection.sink.elem and //connection to self
            connection.sink.index == message.port.index //matching port
        ){
            message_i.port = connection.src;
            found = true;
        }

        if (
            message.action == TopologyResolvePortsMessage::SINK and
            not connection.src.elem and //connection to self
            connection.src.index == message.port.index //matching port
        ){
            message_i.port = connection.sink;
            found = true;
        }

        if (found and message_i.port.elem->is_block())
        {
            ports.push_back(message_i.port);
        }
        else if (found)
        {
            std::vector<Port> ports_i;
            message_i.action = message.action;
            message_i.result = &ports_i;
            Theron::Receiver receiver;
            message_i.port.elem->actor.Push(message_i, receiver.GetAddress());
            receiver.Wait();
            extend(ports, ports_i);
        }
    }

    this->Send(message, from); //ACK
}

void TopologyActor::handle_resolve_conns(
    const TopologyResolveConnectionsMessage &message,
    const Theron::Address from
){
    std::vector<Connection> &real_connections = *message.result;
    real_connections.clear();

    BOOST_FOREACH(Connection &connection, this->connections)
    {
        //ignore connections to self
        if (not connection.src.elem) continue;
        if (not connection.sink.elem) continue;

        std::vector<Port> srcs;
        std::vector<Port> sinks;

        //use the port if its a block
        if (connection.src.elem->is_block())
        {
            srcs.push_back(connection.src);
        }
        //otherwise traverse the port
        {
            Theron::Receiver receiver;
            TopologyResolvePortsMessage message;
            message.action = TopologyResolvePortsMessage::SRC;
            message.port = connection.src;
            message.result = &srcs;
            message.port.elem->actor.Push(message, receiver.GetAddress());
            receiver.Wait();
        }

        //use the port if its a block
        if (connection.sink.elem->is_block())
        {
            sinks.push_back(connection.sink);
        }
        //otherwise traverse the port
        {
            Theron::Receiver receiver;
            TopologyResolvePortsMessage message;
            message.action = TopologyResolvePortsMessage::SINK;
            message.port = connection.sink;
            message.result = &sinks;
            message.port.elem->actor.Push(message, receiver.GetAddress());
            receiver.Wait();
        }

        //append all the actual connections resolved here
        BOOST_FOREACH(const Port &src, srcs)
        {
            BOOST_FOREACH(const Port &sink, sinks)
            {
                real_connections.push_back(Connection(src, sink));
            }
        }
    }

    //get a list of unique sub-topolgies
    std::vector<Element> sub_topologies;
    BOOST_FOREACH(const Topology &topology, this->topologies)
    {
        remove_one(sub_topologies, topology);
        sub_topologies.push_back(topology);
    }

    //add to the list of sub-topolgies involved in connections
    BOOST_FOREACH(Connection &connection, this->connections)
    {
        if (connection.src.elem and not connection.src.elem->is_block())
        {
            remove_one(sub_topologies, connection.src.elem);
            sub_topologies.push_back(connection.src.elem);
        }
        if (connection.sink.elem and not connection.sink.elem->is_block())
        {
            remove_one(sub_topologies, connection.sink.elem);
            sub_topologies.push_back(connection.sink.elem);
        }
    }

    //now recurse through all sub-topologies
    BOOST_FOREACH(const Element &topology, sub_topologies)
    {
        Theron::Receiver receiver;
        std::vector<Connection> real_connections_i;
        TopologyResolveConnectionsMessage message_i;
        message_i.result = &real_connections_i;
        topology->actor.Push(message_i, receiver.GetAddress());
        receiver.Wait();
        extend(real_connections, real_connections_i);
    }

    this->Send(message, from); //ACK
}
