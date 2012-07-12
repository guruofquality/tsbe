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
#include <boost/foreach.hpp>

using namespace tsbe;

/***********************************************************************
 * buffer handler
 **********************************************************************/
inline void TaskActor::handle_buffer_message(const TaskBufferMessage &message, const Theron::Address from)
{
    //step 1) push the buffer into the appropriate queue
    task->input_buffer_queues[message.index].push(message.buffer);

    //step 2) call the task callback since the state changed
    task->config.callback(task);
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
    for (size_t i = 0; i < t[index].size(); i++)
    {
        Endpoint ep_i = t[index][i];
        if (ep_i.task == ep.task and ep_i.index == ep.index)
        {
            t[index].erase(t[index].begin()+i);
            break;
        }
    }

    //trim (dont want trailing empty ones)
    while (not t.empty() and t.back().empty())
    {
        t.resize(t.size()-1);
    }
}

inline void TaskActor::handle_connect_message(const TaskConnectMessage &message, const Theron::Address from)
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

    //send a reply to the receiver
    task->framework.Send(message, this->GetAddress(), from);
}

/***********************************************************************
 * task methods
 **********************************************************************/
TaskConfig::TaskConfig(void)
{
    //NOP
}

Task::Task(void)
{
    //NOP
}

Task::Task(const TaskConfig &config)
{
    this->reset(new TaskImpl(config));

    TaskActor::Parameters actor_params;
    actor_params.task = *this;
    (*this)->actor = (*this)->framework.CreateActor<TaskActor>(actor_params);
}


bool Task::has_input_buffer(const size_t index)
{
    //TODO throw bad index
    return not (*this)->input_buffer_queues[index].empty();
}

bool Task::has_output_buffer(const size_t index)
{
    //TODO throw bad index
    return not (*this)->output_buffer_queues[index].empty();
}

Buffer Task::pop_input_buffer(const size_t index)
{
    //TODO throw bad index or empty
    Buffer buff = (*this)->input_buffer_queues[index].front();
    (*this)->input_buffer_queues[index].pop();
    return buff;
}

Buffer Task::pop_output_buffer(const size_t index)
{
    //TODO throw bad index or empty
    Buffer buff = (*this)->output_buffer_queues[index].front();
    (*this)->output_buffer_queues[index].pop();
    return buff;
}

void Task::send_buffer(const Buffer &buff, const size_t index)
{
    //TODO throw bad index
    BOOST_FOREACH(const Endpoint &ep, (*this)->outputs[index])
    {
        TaskBufferMessage message;
        message.buffer = buff;
        message.index = ep.index;
        Theron::Address from_addr = (*this)->actor.GetAddress();
        ep.task->actor.Push(message, from_addr);
    }
}

size_t Task::get_num_inputs(void)
{
    return (*this)->inputs.size();
}

size_t Task::get_num_outputs(void)
{
    return (*this)->outputs.size();
}
