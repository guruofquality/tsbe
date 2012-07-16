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

#include "element_impl.hpp"
#include <boost/foreach.hpp>

using namespace tsbe;

std::vector<Port> ElementImpl::resolve_src_ports(const Port &src)
{
    //check for weird conditions
    if (not src.elem)
    {
        throw std::runtime_error("resolve ports total inception error!");
    }

    //check if this is a normal block
    if (src.elem->is_block())
    {
        return std::vector<Port>(1, src);
    }

    //otherwise, recurse into the sub topology
    std::vector<Port> ports;
    BOOST_FOREACH(Connection &connection, src.elem->connections)
    {
        if (
            not connection.sink.elem and //connection to self
            connection.sink.index == src.index //matching port
        ){
            const std::vector<Port> ports_i = src.elem->resolve_src_ports(connection.sink);
            ports.insert(ports.end(), ports_i.begin(), ports_i.end());
        }
    }

    return ports;
}

std::vector<Port> ElementImpl::resolve_sink_ports(const Port &sink)
{
    //check for weird conditions
    if (not sink.elem)
    {
        throw std::runtime_error("resolve ports total inception error!");
    }

    //check if this is a normal block
    if (sink.elem->is_block())
    {
        return std::vector<Port>(1, sink);
    }

    //otherwise, recurse into the sub topology
    std::vector<Port> ports;
    BOOST_FOREACH(Connection &connection, sink.elem->connections)
    {
        if (
            not connection.src.elem and //connection to self
            connection.src.index == sink.index //matching port
        ){
            const std::vector<Port> ports_i = sink.elem->resolve_sink_ports(connection.src);
            ports.insert(ports.end(), ports_i.begin(), ports_i.end());
        }
    }

    return ports;
}

std::vector<Connection> ElementImpl::squash_connections(void)
{
    std::vector<Connection> connections;

    // traverse the connections in this topology and flatten them
    BOOST_FOREACH(const Connection &connection, this->connections)
    {
        std::vector<Port> srcs = resolve_src_ports(connection.src);
        std::vector<Port> sinks = resolve_src_ports(connection.sink);
        BOOST_FOREACH(const Port &src, srcs)
        {
            BOOST_FOREACH(const Port &sink, sinks)
            {
                connections.push_back(Connection(src, sink));
            }
        }
    }

    //traverse the sub topologies that arent "connected"
    BOOST_FOREACH(const Topology &topology, this->topologies)
    {
        const std::vector<Connection> connections_i = topology->squash_connections();
        connections.insert(connections.end(), connections_i.begin(), connections_i.end());
    }

    return connections;
}
