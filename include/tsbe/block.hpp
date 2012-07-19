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

//! Typedef for a block's incoming port message handler callback
typedef boost::function<void(const size_t, const boost::any &)> PortMsgHandler;

//! Typedef for a block's regular task handler callback
typedef boost::function<void(const Block &)> BlockTaskHandler;

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

    Block(const BlockConfig &config);

    //! Set various properties on this block
    void set_properties(const BlockProperties &props);

    //! Get the current properties settings
    BlockProperties get_properties(void) const;

    //------------------------------------------------------------------
    //-- the methods below are not thread safe
    //-- they are meant to be using with the context of the callback
    //------------------------------------------------------------------

    /*!
     * Get a bitset representing ready inputs.
     * The bit set will be num inputs wide.
     * If bitset[index] == 1, index has a buffer.
     */
    const BitSet& get_inputs_ready(void);

    /*!
     * Get a bitset representing ready outputs.
     * The bit set will be num outputs wide.
     * If bitset[index] == 1, index has a buffer.
     */
    const BitSet& get_outputs_ready(void);

    /*!
     * Get access to the buffer in the front of the queue.
     * The input queue contains filled buffers.
     * Buffer is NULL if the queue was empty.
     */
    Buffer &get_input_buffer(const size_t index);

    /*!
     * Get access to the buffer in the front of the queue.
     * The output queue contains empty buffers.
     * Buffer is NULL if the queue was empty.
     */
    Buffer &get_output_buffer(const size_t index);

    /*!
     * Remove a buffer from the front of the input queue.
     * The input queue contains filled buffers.
     */
    void pop_input_buffer(const size_t index);

    /*!
     * Pop a buffer from the output queue.
     * The output queue contains empty buffers.
     */
    void pop_output_buffer(const size_t index);

    /*!
     * Send a buffer to all subscribed outputs on this port.
     * \param index the output port index
     */
    void send_buffer(const size_t index, const Buffer &buff);

    /*!
     * Send a message to all subscribed outputs on this port.
     * \param index the output port index
     */
    void send_msg(const size_t index, const boost::any &any);

    //! Get the number of input ports (buffer consumption by task)
    size_t get_num_inputs(void);

    //! Get the number of output ports (buffer production by task)
    size_t get_num_outputs(void);

};

} //namespace tsbe

#endif /*INCLUDED_TSBE_BLOCK_HPP*/
