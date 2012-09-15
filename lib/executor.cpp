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

#include <tsbe_impl/common_impl.hpp>
#include <tsbe_impl/executor_impl.hpp>

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
    (*this)->actor = boost::shared_ptr<Actor>(new ExecutorActor(config));
    (*this)->thread_pool = (*this)->actor->get_thread_pool();
}

void Executor::commit(void)
{
    Theron::Receiver receiver;
    ExecutorCommitMessage message;
    (*this)->actor->Push(message, receiver.GetAddress());
    receiver.Wait();
}


void Executor::post_msg(const Wax &msg)
{
    Theron::Receiver receiver;
    ExecutorPostMessage message;
    message.msg = msg;
    (*this)->actor->Push(message, receiver.GetAddress());
    receiver.Wait();
}
