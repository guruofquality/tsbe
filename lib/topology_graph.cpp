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

void ElementImpl::squash(void)
{
    //step 0) squash connections
    squashed_connections = this->squash_connections();

    //step 1) squash blocks
    squashed_blocks.clear();
    BOOST_FOREACH(const Connection &connection, squashed_connections)
    {
        //remove one is the laziest way to ensure unique elements
        //a possible duplicate will always be removed before push_back
        const Block &src = reinterpret_cast<const Block &>(connection.src.elem);
        remove_one(squashed_blocks, src);
        squashed_blocks.push_back(src);
        const Block &sink = reinterpret_cast<const Block &>(connection.sink.elem);
        remove_one(squashed_blocks, sink);
        squashed_blocks.push_back(sink);
    }

    //step 2) get a unique list of group names
    std::vector<std::string> group_names;
    BOOST_FOREACH(const Block &block, squashed_blocks)
    {
        const std::string name = block.get_task_group();
        remove_one(group_names, name);
        group_names.push_back(name);
    }

    //step 3) create task groups
    squashed_tasks.clear();
    BOOST_FOREACH(const std::string &name, group_names)
    {
        TaskGroup tg;
        tg.name = name;
        BOOST_FOREACH(const Block &block, squashed_blocks)
        {
            if (block.get_task_group() == name)
            {
                tg.blocks.push_back(block);
            }
        }
        //TODO set tg.task w/ callback etc
    }

    //step 4) create the flows
    BOOST_FOREACH(const Connection &connection, squashed_connections)
    {
        TaskGroup src_tg;
        TaskGroup sink_tg;
    }
}
