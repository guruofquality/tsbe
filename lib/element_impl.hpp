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

#ifndef INCLUDED_LIBTSBE_ELEMENT_IMPL_HPP
#define INCLUDED_LIBTSBE_ELEMENT_IMPL_HPP

#include <tsbe/topology.hpp>
#include <tsbe/block.hpp>
#include <tsbe/task.hpp>
#include <tsbe/flow.hpp>
#include <vector>

namespace tsbe
{

struct TaskGroup
{
    std::string name;
    std::vector<Block> blocks;
    Task task;
};

//! ElementImpl is both a topology and a block to allow interconnection
struct ElementImpl
{
    ElementImpl(const BlockConfig &config)
    {
        block_config = config;
        block = true;
    }

    ElementImpl(const TopologyConfig &config)
    {
        topology_config = config;
        block = false;
    }

    bool block;
    bool is_block(void) const {return block;}

    //--------- block stuff
    BlockConfig block_config;
    std::string group;

    //TODO determine this stuff when squashing, we dont need to update it here
    std::vector<std::vector<Port> > output_endpoints;
    std::vector<std::vector<Port> > input_endpoints;

    //--------- topology stuff
    TopologyConfig topology_config;
    std::vector<Topology> topologies;
    std::vector<Connection> connections;

    //recursive helpers
    std::vector<Port> resolve_src_ports(const Port &);
    std::vector<Port> resolve_sink_ports(const Port &);
    std::vector<Connection> squash_connections(void);

    //the result of a good squashing
    std::vector<Connection> squashed_connections;
    std::vector<Block> squashed_blocks;
    std::vector<TaskGroup> squashed_tasks;
    std::vector<Flow> squashed_flows;
    void squash(void);
};

} //namespace tsbe

#endif /*INCLUDED_LIBTSBE_ELEMENT_IMPL_HPP*/
