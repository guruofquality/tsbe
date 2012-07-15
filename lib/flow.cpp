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

#include "task_impl.hpp"

using namespace tsbe;

FlowConfig::FlowConfig(void)
{
    source_index = 0;
    dest_index = 0;
}

struct tsbe::FlowImpl
{
    FlowImpl(const FlowConfig &config):
        config(config)
    {
        this->create();
    }

    ~FlowImpl(void)
    {
        this->destroy();
    }

    void create(void);

    void destroy(void);

    const FlowConfig config;
};

void FlowImpl::create(void)
{
    Theron::Receiver receiver;
    TaskConnectMessage message;
    message.connection = config;

    //tell the destination first
    message.action = TaskConnectMessage::CONNECT_DEST;
    config.dest_task->actor.Push(message, receiver.GetAddress());
    receiver.Wait();
    message.action = TaskConnectMessage::CONNECT_SOURCE;
    config.source_task->actor.Push(message, receiver.GetAddress());
    receiver.Wait();
}

void FlowImpl::destroy(void)
{
    Theron::Receiver receiver;
    TaskConnectMessage message;
    message.connection = config;

    //tell the source first
    message.action = TaskConnectMessage::DISCONNECT_SOURCE;
    config.source_task->actor.Push(message, receiver.GetAddress());
    receiver.Wait();
    message.action = TaskConnectMessage::DISCONNECT_DEST;
    config.dest_task->actor.Push(message, receiver.GetAddress());
    receiver.Wait();
}

Flow::Flow(void)
{
    //NOP
}

Flow::Flow(const FlowConfig &config)
{
    this->reset(new FlowImpl(config));
}

Task Flow::get_source_task(void) const
{
    return (*this)->config.source_task;
}

size_t Flow::get_source_index(void) const
{
    return (*this)->config.source_index;
}

Task Flow::get_dest_task(void) const
{
    return (*this)->config.dest_task;
}

size_t Flow::get_dest_index(void) const
{
    return (*this)->config.dest_index;
}
