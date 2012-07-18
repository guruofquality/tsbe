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

#ifndef INCLUDED_LIBTSBE_TASK_IMPL_HPP
#define INCLUDED_LIBTSBE_TASK_IMPL_HPP

#include <tsbe/config.hpp>
#include <tsbe/task.hpp>
#include <tsbe/flow.hpp>
#include <Theron/Framework.h>
#include <Theron/Actor.h>
#include <Theron/ActorRef.h>
#include <vector>
#include <queue>

namespace tsbe
{

/***********************************************************************
 * Messages to be accepted by the task's actor
 **********************************************************************/
struct TaskBufferMessage
{
    Buffer buffer;
    size_t index;
    bool input;
};

struct TaskConnectMessage
{
    FlowConfig connection;
    enum {CONNECT_SOURCE, CONNECT_DEST, DISCONNECT_SOURCE, DISCONNECT_DEST} action;
};


/***********************************************************************
 * The actor that will run inside the task itself
 **********************************************************************/
struct TaskActor : Theron::Actor
{
    struct Parameters
    {
        TaskImpl *self;
    };

    inline explicit TaskActor(const Parameters &params)
    {
        self = params.self;
        RegisterHandler(this, &TaskActor::handle_buffer_message);
        RegisterHandler(this, &TaskActor::handle_connect_message);
        RegisterHandler(this, &TaskActor::handle_affinity_message);
    }

    void handle_buffer_message(const TaskBufferMessage &message, const Theron::Address from);
    void handle_connect_message(const TaskConnectMessage &message, const Theron::Address from);
    void handle_affinity_message(const Affinity &message, const Theron::Address from);

    TaskImpl *self;
};

/***********************************************************************
 * Impl storage for private members of the task
 **********************************************************************/
struct TaskDestination
{
    Theron::ActorRef actor; //the actor to push to
    size_t index; //the index of actor's input port
};

struct TaskImpl
{
    TaskImpl(const TaskConfig &config):
        config(config),
        framework(1/*thread*/)
    {
        //NOP
    }

    Task *ptr_self;

    BitSet inputs_ready;
    BitSet outputs_ready;

    const TaskConfig config;
    std::vector<std::queue<Buffer> > input_buffer_queues;
    std::vector<std::queue<Buffer> > output_buffer_queues;

    std::vector<std::vector<FlowEndpoint> > inputs;
    std::vector<std::vector<FlowEndpoint> > outputs;

    Theron::Framework framework;
    Theron::ActorRef actor;

    Affinity affinity;
};

} //namespace tsbe

#endif /*INCLUDED_LIBTSBE_TASK_IMPL_HPP*/
