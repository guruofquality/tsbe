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

#ifndef INCLUDED_LIBTSBE_TOPOLOGY_IMPL_HPP
#define INCLUDED_LIBTSBE_TOPOLOGY_IMPL_HPP

#include "element_impl.hpp"
#include <tsbe/buffer.hpp>
#include <tsbe/topology.hpp>
#include <Theron/Framework.h>
#include <Theron/Actor.h>
#include <Theron/ActorRef.h>
#include <vector>
#include <queue>

namespace tsbe
{

/***********************************************************************
 * Messages that can be sent to this actor
 **********************************************************************/
struct TopologyConnectMessage
{
    enum {CONNECT, DISCONNECT, ADD, REMOVE} action;
    Connection connection;
    Topology topology;
};

struct TopologyResolvePortsMessage
{
    enum {SRC, SINK} action;
    Port port;
    std::vector<Port> *result;
};

struct TopologyResolveConnectionsMessage
{
    std::vector<Connection> *result;
};

/***********************************************************************
 * The details of the topology actor
 **********************************************************************/
struct TopologyActor : Theron::Actor
{
    inline explicit TopologyActor(void)
    {
        RegisterHandler(this, &TopologyActor::handle_connect);
        RegisterHandler(this, &TopologyActor::handle_resolve_ports);
        RegisterHandler(this, &TopologyActor::handle_resolve_conns);
    }

    void handle_connect(const TopologyConnectMessage &message, const Theron::Address from);
    void handle_resolve_ports(const TopologyResolvePortsMessage &message, const Theron::Address from);
    void handle_resolve_conns(const TopologyResolveConnectionsMessage &message, const Theron::Address from);

    std::vector<Topology> topologies;
    std::vector<Connection> connections;
    std::vector<Connection> flat_connections;
};

} //namespace tsbe

#endif /*INCLUDED_LIBTSBE_TOPOLOGY_IMPL_HPP*/
