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
        self->input_buffer_queues[message.index].push(message.buffer);
        self->inputs_ready.set(message.index);
    }
    else
    {
        self->output_buffer_queues[message.index].push(message.buffer);
        self->outputs_ready.set(message.index);
    }

    //step 2) call the task callback since the state changed
    self->config.callback(*self->ptr_self);
}

/***********************************************************************
 * connect handler
 **********************************************************************/
void TaskActor::handle_connect_message(const TaskConnectMessage &message, const Theron::Address from)
{
    const FlowEndpoint &dest_ep = message.connection.dest;
    const FlowEndpoint &source_ep = message.connection.source;

    switch(message.action)
    {
    case TaskConnectMessage::CONNECT_SOURCE:
        vector_vector_add(self->outputs, dest_ep, message.connection.source.index);
        break;

    case TaskConnectMessage::CONNECT_DEST:
        vector_vector_add(self->inputs, source_ep, message.connection.dest.index);
        break;

    case TaskConnectMessage::DISCONNECT_SOURCE:
        vector_vector_remove(self->outputs, dest_ep, message.connection.source.index);
        break;

    case TaskConnectMessage::DISCONNECT_DEST:
        vector_vector_remove(self->inputs, source_ep, message.connection.dest.index);
        break;

    }

    //always resize the input queues, they should match with the connections
    self->output_buffer_queues.resize(self->outputs.size());
    self->input_buffer_queues.resize(self->inputs.size());

    //also resize the ready bitsets to match
    self->outputs_ready.resize(self->outputs.size());
    self->inputs_ready.resize(self->inputs.size());

    //send a reply to the receiver
    self->framework.Send(message, this->GetAddress(), from);
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
