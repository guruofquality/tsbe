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
    BOOST_FOREACH(const Connection &conn, connections_to_remove)
    {
        //TODO
    }

    //step 3) create new connections
    const std::vector<Connection> connections_to_add = vector_subtract(new_flat_connections, this->flat_connections);
    BOOST_FOREACH(const Connection &conn, connections_to_add)
    {
        //TODO
    }

    //step 4) update the flat connections
    this->flat_connections = new_flat_connections;

    this->Send(message, from); //ACK
}
