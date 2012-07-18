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
#include <Theron/Framework.h>
#include <Theron/Actor.h>
#include <Theron/ActorRef.h>
#include <vector>
#include <queue>

namespace tsbe
{

struct TopologyConnectMessage
{
    enum {CONNECT, DISCONNECT, ADD, REMOVE} action;
    ElementImpl *caller;
    Connection connection;
    Topology topology;
};

/***********************************************************************
 * The actor that will run inside the element itself
 **********************************************************************/
struct ElementActor : Theron::Actor
{
    struct Parameters
    {
        ElementImpl *self;
    };

    inline explicit ElementActor(const Parameters &params)
    {
        self = params.self;
        RegisterHandler(this, &ElementActor::handle_connect);
    }

    void handle_connect(const TopologyConnectMessage &message, const Theron::Address from);

    ElementImpl *self;
};

struct TaskGroup
{
    std::string name;
    std::vector<Block> blocks;
    Task task;
};

//! ElementImpl is both a topology and a block to allow interconnection
struct ElementImpl
{
    ElementImpl(const BlockConfig &config):
        framework(1/*thread*/)
    {
        block_config = config;
        block = true;
    }

    ElementImpl(const TopologyConfig &config):
        framework(1/*thread*/)
    {
        topology_config = config;
        block = false;
    }

    boost::weak_ptr<ElementImpl> weak_self;
    boost::weak_ptr<ElementImpl> parent;
    Theron::Framework framework;
    Theron::ActorRef actor;

    bool block;
    bool is_block(void) const {return block;}

    //--------- block stuff
    BlockConfig block_config;
    std::string group;

    std::vector<std::vector<Port> > inputs;
    std::vector<std::vector<Port> > outputs;

    std::vector<std::queue<Buffer> > input_buffer_queues;
    std::vector<std::queue<Buffer> > output_buffer_queues;

    //--------- topology stuff
    TopologyConfig topology_config;
    std::vector<Topology> topologies;
    std::vector<Connection> connections;

    //recursive helpers
    std::vector<Port> resolve_src_ports(const Port &);
    std::vector<Port> resolve_sink_ports(const Port &);
    std::vector<Connection> resolve_connections(void);
    void reparent(void);

    //the result of a good squashing
    std::vector<Connection> flat_connections;
    //std::vector<Block> squashed_blocks;
    //std::vector<TaskGroup> squashed_tasks;
    //std::vector<Flow> squashed_flows;
    void update(void);
};

} //namespace tsbe

#endif /*INCLUDED_LIBTSBE_ELEMENT_IMPL_HPP*/
