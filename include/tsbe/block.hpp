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
#include <boost/any.hpp>
#include <boost/function.hpp>
#include <string>

namespace tsbe
{

struct BlockProperties
{
    std::string task_group;
    size_t task_priority;
};

typedef boost::function<void(const boost::any &)> BlockMsgHandler;

struct InputPattern
{
    size_t history;
    size_t multiple;
};

struct OutputPattern
{
    size_t multiple;
};

struct TSBE_API BlockConfig
{
    BlockConfig(void);
};

struct TSBE_API Block : boost::shared_ptr<ElementImpl>
{
    //! Construct a null block
    Block(void);

    Block(const BlockConfig &config);

    //! Get the number of input ports
    size_t get_num_inputs(void);

    //! Get the number of output ports
    size_t get_num_outputs(void);

    //! Set the associated task group, or empty for None
    void set_task_group(const std::string &group);

    //! Get the associated task group
    std::string get_task_group(void) const;

    //! Set a property like task group or group priority
    void set_property(const std::string &key, const std::string &value);

    //! How many bytes consumed on the given input port?
    unsigned long long get_bytes_consumed(const size_t index);

    //! How many bytes produced on the given output port?
    unsigned long long get_bytes_produced(const size_t index);

    //! Send a message to all downstream subscribers
    void send_msg(const size_t index, const boost::any &any);

    //! Register a handler to be called for incoming messages
    void register_msg_handler(const size_t index, const BlockMsgHandler &handler);

};

} //namespace tsbe

#endif /*INCLUDED_TSBE_BLOCK_HPP*/
