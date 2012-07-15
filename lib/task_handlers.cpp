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
#include <numanuma.hpp>

using namespace tsbe;

/***********************************************************************
 * buffer handler
 **********************************************************************/
void TaskActor::handle_buffer_message(const TaskBufferMessage &message, const Theron::Address from)
{
    //step 1) push the buffer into the appropriate queue
    if (message.input)
    {
        task->input_buffer_queues[message.index].push(message.buffer);
        task->inputs_ready.set(message.index);
    }
    else
    {
        task->output_buffer_queues[message.index].push(message.buffer);
        task->outputs_ready.set(message.index);
    }

    //step 2) call the task callback since the state changed
    task->config.callback(*task4cb);
}

/***********************************************************************
 * connect handler + helpers
 **********************************************************************/
template <typename T>
void connect(T &t, Endpoint ep, const size_t index)
{
    //ensure that there is room
    if (index >= t.size())
    {
        t.resize(index+1);
    }

    //add the endpoint
    t[index].push_back(ep);
}

template <typename T>
void disconnect(T &t, Endpoint ep, const size_t index)
{
    //remove the first match found for this connection
    remove_one(t[index], ep);

    //trim (dont want trailing empty ones)
    while (not t.empty() and t.back().empty())
    {
        t.resize(t.size()-1);
    }
}

void TaskActor::handle_connect_message(const TaskConnectMessage &message, const Theron::Address from)
{
    Endpoint dest_ep;
    dest_ep.task = message.connection.dest_task;
    dest_ep.index = message.connection.dest_index;

    Endpoint source_ep;
    source_ep.task = message.connection.source_task;
    source_ep.index = message.connection.source_index;

    switch(message.action)
    {
    case TaskConnectMessage::CONNECT_SOURCE:
        connect(task->outputs, dest_ep, message.connection.source_index);
        break;

    case TaskConnectMessage::CONNECT_DEST:
        connect(task->inputs, source_ep, message.connection.dest_index);
        break;

    case TaskConnectMessage::DISCONNECT_SOURCE:
        disconnect(task->outputs, dest_ep, message.connection.source_index);
        break;

    case TaskConnectMessage::DISCONNECT_DEST:
        disconnect(task->inputs, source_ep, message.connection.dest_index);
        break;

    }

    //always resize the input queues, they should match with the connections
    task->output_buffer_queues.resize(task->outputs.size());
    task->input_buffer_queues.resize(task->inputs.size());

    //also resize the ready bitsets to match
    task->outputs_ready.resize(task->outputs.size());
    task->inputs_ready.resize(task->inputs.size());

    //send a reply to the receiver
    task->framework.Send(message, this->GetAddress(), from);
}

/***********************************************************************
 * setting the affinity
 **********************************************************************/
void TaskActor::handle_affinity_message(const Affinity &message, const Theron::Address from)
{
    Affinity affinity = message;
    if (affinity == Affinity())
    {
        numanuma::set_thread_affinity(NUMANUMA_NODE_NONE);
    }
    else
    {
        numanuma::set_thread_affinity(affinity);
    }
}
