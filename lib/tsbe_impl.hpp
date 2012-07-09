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

#ifndef INCLUDED_LIBTSBE_IMPL_HPP
#define INCLUDED_LIBTSBE_IMPL_HPP

#include <tsbe/config.hpp>
#include <tsbe/task.hpp>
#include <tsbe/connection.hpp>
#include <Theron/Framework.h>
#include <Theron/Actor.h>
#include <Theron/ActorRef.h>
#include <vector>
#include <queue>

/***********************************************************************
 * Messages to be accepted by the task's actor
 **********************************************************************/
struct TaskBufferMessage
{
    tsbe::Buffer buffer;
    size_t index;
};

struct TaskConnectMessage
{
    tsbe::ConnectionConfig connection;
    bool create;
};

/***********************************************************************
 * The actor that will run inside the task itself
 **********************************************************************/
struct TaskActor : Theron::Actor
{

    struct Parameters
    {
        tsbe::Task task;
    };

    inline explicit TaskActor(const Parameters &params)
    {
        task = params.task;
        RegisterHandler(this, &TaskActor::handle_buffer_message);
        RegisterHandler(this, &TaskActor::handle_connect_message);
    }

    void handle_buffer_message(const TaskBufferMessage &message, const Theron::Address from);
    void handle_connect_message(const TaskConnectMessage &message, const Theron::Address from);

    tsbe::Task task;
};

/***********************************************************************
 * Impl storage for private members of the task
 **********************************************************************/
struct TaskDestination
{
    Theron::ActorRef actor; //the actor to push to
    size_t index; //the index of actor's input port
};

struct tsbe::TaskImpl
{
    TaskImpl(const tsbe::TaskConfig &config):
        config(config),
        framework(config.num_threads)
    {
        //NOP
    }

    const tsbe::TaskConfig config;
    std::vector<std::queue<Buffer> > input_buffer_queues;
    std::vector<std::vector<TaskDestination> > output_destinations;

    Theron::Framework framework;
    Theron::ActorRef actor;
};



#endif /*INCLUDED_LIBTSBE_IMPL_HPP*/
