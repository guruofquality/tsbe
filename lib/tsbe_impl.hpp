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

#include <tsbe/task.hpp>
#include <tsbe/buffer.hpp>
#include <Theron/Framework.h>
#include <Theron/Actor.h>
#include <Theron/ActorRef.h>
#include <vector>
#include <queue>

using namespace tsbe;

struct TaskBufferMessage
{
    Buffer buffer;
    size_t index;
};

struct TaskActor : Theron::Actor
{

    struct Parameters
    {
        Task task;
    };

    inline explicit TaskActor(const Parameters &params)
    {
        task = params.task;
        RegisterHandler(this, &TaskActor::handle_buffer_message);
    }

    void handle_buffer_message(const TaskBufferMessage &message, const Theron::Address from);

    Task task;
};

struct TaskDestination
{
    Theron::ActorRef actor; //the actor to push to
    size_t index; //the index of actor's input port
};

struct tsbe::TaskImpl
{
    TaskImpl(void):
        framework(1) //only want 1 thread
    {
        //NOP
    }

    TaskConfig config;
    std::vector<std::queue<Buffer> > input_buffer_queues;
    std::vector<std::vector<TaskDestination> > output_destinations;

    Theron::Framework framework;
    Theron::ActorRef actor;
};

inline void TaskActor::handle_buffer_message(const TaskBufferMessage &message, const Theron::Address from)
{
    //step 1) push the buffer into the appropriate queue
    task->input_buffer_queues[message.index].push(message.buffer);

    //step 2) call the task callback since the state changed
    task->config.callback(task);
}
