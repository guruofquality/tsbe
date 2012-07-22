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

#include "element_impl.hpp"
#include "task_interface_impl.hpp"
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

/***********************************************************************
 * The details of the block actor
 **********************************************************************/
struct BlockActor : Theron::Actor
{
    inline explicit BlockActor(void)
    {
        RegisterHandler(this, &BlockActor::handle_connect);
        interface.reset(new TaskInterfaceImpl());
    }

    void handle_connect(const BlockConnectMessage &message, const Theron::Address from);

    TaskInterface interface;
};

} //namespace tsbe

#endif /*INCLUDED_LIBTSBE_BLOCK_IMPL_HPP*/
