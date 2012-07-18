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
#include "vec_utils.hpp"

using namespace tsbe;

void ElementActor::handle_connect(const TopologyConnectMessage &message, const Theron::Address from)
{
    boost::shared_ptr<ElementImpl> parent = self->parent.lock();
    if (parent)
    {
        parent->actor.Push(message, from);
        
        //TODO use receiver
        return;
    }

    //remove instances of this for outside connections to avoid circular shared ptrs
    Connection connection = message.connection;
    if (connection.src.elem.get() == message.caller) connection.src.elem.reset();
    if (connection.sink.elem.get() == message.caller) connection.sink.elem.reset();

    switch(message.action)
    {
    case TopologyConnectMessage::CONNECT:
        message.caller->connections.push_back(connection);
        break;

    case TopologyConnectMessage::DISCONNECT:
        //TODO remove parentage, it will be updated below
        if (not remove_one(message.caller->connections, connection))
        {
            //TODO pass error, dont throw here
            throw std::runtime_error("Topology::disconnect could not find connection");
        }
        break;

    case TopologyConnectMessage::ADD:
        message.caller->topologies.push_back(message.topology);
        break;

    case TopologyConnectMessage::REMOVE:
        //TODO remove parentage, it will be updated below
        if (not remove_one(message.caller->topologies, message.topology))
        {
            //TODO pass error, dont throw here
            throw std::runtime_error("Topology::remove_topology could not find topology");
        }
        break;
    }

    //call update after any changes to squash the hierarchy
    self->update();

    self->framework.Send(message, this->GetAddress(), from); //ACK
}
