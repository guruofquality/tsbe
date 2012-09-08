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

#ifndef INCLUDED_LIBTSBE_EXECUTOR_IMPL_HPP
#define INCLUDED_LIBTSBE_EXECUTOR_IMPL_HPP

#include <tsbe/executor.hpp>
#include <Theron/Framework.h>
#include <Theron/Actor.h>
#include <vector>

namespace tsbe
{

/***********************************************************************
 * Messages that can be sent to this actor
 **********************************************************************/
struct ExecutorCommitMessage
{
    //empty
};

struct ExecutorPostMessage
{
    Wax msg;
};

/***********************************************************************
 * The details of the execturor actor
 **********************************************************************/
struct ExecutorActor : Theron::Actor
{
    inline explicit ExecutorActor(Theron::Framework &framework, const ExecutorConfig &config):
        Theron::Actor(framework), config(config)
    {
        RegisterHandler(this, &ExecutorActor::handle_commit);
        RegisterHandler(this, &ExecutorActor::handle_post_msg);
    }

    ~ExecutorActor(void)
    {
        flat_connections.clear();
        block_set.clear();
    }

    void handle_commit(const ExecutorCommitMessage &message, const Theron::Address from);
    void handle_post_msg(const ExecutorPostMessage &message, const Theron::Address from);

    std::vector<Connection> flat_connections;
    std::vector<Element> block_set;

    ExecutorConfig config;
    Theron::Receiver receiver;

    void send_topology_update(const std::vector<Element> &);
};

/***********************************************************************
 * The details of the execturor impl
 **********************************************************************/
struct ExecutorImpl
{
    ExecutorImpl(void):
        framework(1/*thread*/)
    {
        //NOP
    }

    Theron::Framework framework;
    boost::shared_ptr<Theron::Actor> actor;
};

} //namespace tsbe

#endif /*INCLUDED_LIBTSBE_EXECUTOR_IMPL_HPP*/
