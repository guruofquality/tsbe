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

#include <tsbe_impl/thread_pool_impl.hpp>
#include <tsbe_impl/common_impl.hpp>
#include <tsbe_impl/topology_impl.hpp>

using namespace tsbe;

TopologyConfig::TopologyConfig(void)
{
    //NOP
}

Topology::Topology(void)
{
    //NOP
}

Topology::Topology(const TopologyConfig &)
{
    this->reset(new ElementImpl());
    (*this)->block = false;
    (*this)->thread_pool = ThreadPool::get_active();
    //(*this)->framework = boost::shared_ptr<Theron::Framework>(new Theron::Framework(1/*thread*/));
    (*this)->actor = boost::shared_ptr<Actor>(new TopologyActor((*this)->thread_pool->framework));
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
    (*this)->actor->Push(message, receiver.GetAddress());
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
    (*this)->actor->Push(message, receiver.GetAddress());
    receiver.Wait();
}

void Topology::remove_topology(const Topology &topology)
{
    Theron::Receiver receiver;
    TopologyConnectMessage message;
    message.action = TopologyConnectMessage::REMOVE;
    message.topology = topology;
    (*this)->actor->Push(message, receiver.GetAddress());
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
    (*this)->actor->Push(message, receiver.GetAddress());
    receiver.Wait();
}

void Topology::clear_all(void)
{
    Theron::Receiver receiver;
    TopologyClearMessage message;
    (*this)->actor->Push(message, receiver.GetAddress());
    receiver.Wait();
}
