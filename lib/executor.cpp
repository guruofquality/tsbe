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

using namespace tsbe;

ExecutorConfig::ExecutorConfig(void)
{
    //NOP
}

Executor::Executor(void)
{
    //NOP
}

Executor::Executor(const ExecutorConfig &config)
{
    this->reset(new ExecutorImpl());
    ExecutorActor::Parameters params;
    params.config = config;
    (*this)->actor = (*this)->framework.CreateActor<ExecutorActor>(params);
}

void Executor::update(void)
{/*
    //call the resolve connections to flatten
    {
        Theron::Receiver receiver;
        TopologyResolveConnectionsMessage message;
        std::vector<Connection> flat_connections;
        message.result = &flat_connections;
        (*this)->actor.Push(message, receiver.GetAddress());
        receiver.Wait();
    }

    //actually perform the updating of connections
    {
        Theron::Receiver receiver;
        TopologyUpdateMessage message;
        (*this)->actor.Push(message, receiver.GetAddress());
        receiver.Wait();
    }
*/}
