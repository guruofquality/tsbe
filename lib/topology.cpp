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
#include <boost/foreach.hpp>

using namespace tsbe;

TopologyConfig::TopologyConfig(void)
{
    //NOP
}

Port::Port(void)
{
    index = 0;
}

Port::Port(const Element &elem, const size_t index):
    elem(elem), index(index)
{
    //NOP
}

bool tsbe::operator==(const Port &lhs, const Port &rhs)
{
    return (lhs.elem == rhs.elem and lhs.index == rhs.index);
}

Connection::Connection(void)
{
    //NOP
}

Connection::Connection(const Port &src, const Port &sink):
    src(src), sink(sink)
{
    //NOP
}

bool tsbe::operator==(const Connection &lhs, const Connection &rhs)
{
    return (lhs.src == rhs.src and lhs.sink == rhs.sink);
}

Topology::Topology(void)
{
    //NOP
}

Topology::Topology(const TopologyConfig &)
{
    this->reset(new ElementImpl());
    (*this)->actor = (*this)->framework.CreateActor<TopologyActor>();
}

const Element &Topology::self(void) const
{
    return *this;
}

void Topology::add_topology(const Topology &topology)
{
    Theron::Receiver receiver;
    TopologyConnectMessage message;
    message.action = TopologyConnectMessage::ADD;
    message.topology = topology;
    (*this)->actor.Push(message, receiver.GetAddress());
    receiver.Wait();
}

void Topology::connect(const Connection &connection_)
{
    //remove instances of this for outside connections to avoid circular shared ptrs
    Connection connection = connection_;
    if (connection.src.elem.get() == this->get()) connection.src.elem.reset();
    if (connection.sink.elem.get() == this->get()) connection.sink.elem.reset();

    Theron::Receiver receiver;
    TopologyConnectMessage message;
    message.action = TopologyConnectMessage::CONNECT;
    message.connection = connection;
    (*this)->actor.Push(message, receiver.GetAddress());
    receiver.Wait();
}

void Topology::remove_topology(const Topology &topology)
{
    Theron::Receiver receiver;
    TopologyConnectMessage message;
    message.action = TopologyConnectMessage::REMOVE;
    message.topology = topology;
    (*this)->actor.Push(message, receiver.GetAddress());
    receiver.Wait();
}

void Topology::disconnect(const Connection &connection_)
{
    //remove instances of this for outside connections to avoid circular shared ptrs
    Connection connection = connection_;
    if (connection.src.elem.get() == this->get()) connection.src.elem.reset();
    if (connection.sink.elem.get() == this->get()) connection.sink.elem.reset();

    Theron::Receiver receiver;
    TopologyConnectMessage message;
    message.action = TopologyConnectMessage::DISCONNECT;
    message.connection = connection;
    (*this)->actor.Push(message, receiver.GetAddress());
    receiver.Wait();
}

void Topology::update(void)
{
    //TODO lol
}
