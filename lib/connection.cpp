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

#include "tsbe_impl.hpp"

using namespace tsbe;

ConnectionConfig::ConnectionConfig(void)
{
    source_index = 0;
    dest_index = 0;
}

struct tsbe::ConnectionImpl
{
    ConnectionImpl(const ConnectionConfig &config):
        config(config)
    {
        this->create();
    }

    ~ConnectionImpl(void)
    {
        this->destroy();
    }

    void create(void);

    void destroy(void);

    const ConnectionConfig config;
};

void ConnectionImpl::create(void)
{
    Theron::Receiver receiver;
    TaskConnectMessage message;
    message.connection = config;
    message.create = true;

    //tell the destination first
    config.dest_task->actor.Push(message, receiver.GetAddress());
    receiver.Wait();
    config.source_task->actor.Push(message, receiver.GetAddress());
    receiver.Wait();
}

void ConnectionImpl::destroy(void)
{
    Theron::Receiver receiver;
    TaskConnectMessage message;
    message.connection = config;
    message.create = false;

    //tell the source first
    config.source_task->actor.Push(message, receiver.GetAddress());
    receiver.Wait();
    config.dest_task->actor.Push(message, receiver.GetAddress());
    receiver.Wait();
}

Connection::Connection(void)
{
    //NOP
}

Connection::Connection(const ConnectionConfig &config)
{
    this->reset(new ConnectionImpl(config));
}

Task Connection::get_source_task(void) const
{
    return (*this)->config.source_task;
}

size_t Connection::get_source_index(void) const
{
    return (*this)->config.source_index;
}

Task Connection::get_dest_task(void) const
{
    return (*this)->config.dest_task;
}

size_t Connection::get_dest_index(void) const
{
    return (*this)->config.dest_index;
}
