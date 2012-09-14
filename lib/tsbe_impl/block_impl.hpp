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

#ifndef INCLUDED_LIBTSBE_BLOCK_IMPL_HPP
#define INCLUDED_LIBTSBE_BLOCK_IMPL_HPP

#include <tsbe_impl/thread_pool_impl.hpp>
#include <tsbe_impl/element_impl.hpp>
#include <tsbe_impl/task_interface_impl.hpp>
#include <tsbe/connection.hpp>
#include <tsbe/block.hpp>
#include <vector>
#include <queue>

namespace tsbe
{

/***********************************************************************
 * Messages that can be sent to this actor
 **********************************************************************/
struct BlockConnectMessage
{
    enum {SRC_CON, SINK_CON, SRC_DIS, SINK_DIS} action;
    Connection connection;
};

struct BlockChangedMessage
{
    //empty
};

struct BlockInputMessage
{
    Wax msg;
    size_t index;
};

struct BlockOutputMessage
{
    Wax msg;
    size_t index;
};

struct BlockPostMessage
{
    Wax msg;
};

/***********************************************************************
 * The details of the block actor
 **********************************************************************/
struct BlockActor : Theron::Actor
{
    inline explicit BlockActor(Theron::Framework &framework, const BlockConfig &config):
        Theron::Actor(framework), config(config)
    {
        task_iface.reset(new TaskInterfaceImpl());
        RegisterHandler(this, &BlockActor::handle_connect);
        RegisterHandler(this, &BlockActor::handle_input_msg);
        RegisterHandler(this, &BlockActor::handle_output_msg);
        RegisterHandler(this, &BlockActor::handle_post_msg);
        RegisterHandler(this, &BlockActor::handle_changed);
    }

    void handle_connect(const BlockConnectMessage &message, const Theron::Address from);
    void handle_input_msg(const BlockInputMessage &message, const Theron::Address from);
    void handle_output_msg(const BlockOutputMessage &message, const Theron::Address from);
    void handle_post_msg(const BlockPostMessage &message, const Theron::Address from);
    void handle_changed(const BlockChangedMessage &message, const Theron::Address from);

    TaskInterface task_iface;
    BlockConfig config;
};

} //namespace tsbe

#endif /*INCLUDED_LIBTSBE_BLOCK_IMPL_HPP*/
