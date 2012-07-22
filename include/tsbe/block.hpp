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

#ifndef INCLUDED_TSBE_BLOCK_HPP
#define INCLUDED_TSBE_BLOCK_HPP

#include <tsbe/config.hpp>
#include <tsbe/task_interface.hpp>
#include <boost/any.hpp>
#include <boost/function.hpp>
#include <string>

namespace tsbe
{

//! Typedef for a block's incoming port message handler callback
typedef boost::function<void(const size_t, const boost::any &)> PortMsgHandler;

//! Typedef for a block's regular task handler callback
typedef boost::function<void(const TaskInterface &)> BlockTaskHandler;

struct TSBE_API BlockProperties
{
    std::string task_group;
    size_t task_priority;
};

struct TSBE_API BlockConfig
{
    BlockConfig(void);
    PortMsgHandler port_msg_handler;
    BlockTaskHandler block_task_handler;
};

struct TSBE_API Block : boost::shared_ptr<ElementImpl>
{
    //! Construct a null block
    Block(void);

    //! Create a new block from config params
    Block(const BlockConfig &config);

    //! Set various properties on this block
    void set_properties(const BlockProperties &props);

    //! Get the current properties settings
    BlockProperties get_properties(void) const;
};

} //namespace tsbe

#endif /*INCLUDED_TSBE_BLOCK_HPP*/
