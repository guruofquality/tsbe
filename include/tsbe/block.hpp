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
#include <tsbe/buffer.hpp>
#include <tsbe/task_interface.hpp>
#include <tsbe/wax.hpp>
#include <boost/function.hpp>

namespace tsbe
{

//! Typedef for a block's incoming port message handler callback
typedef boost::function<void(const TaskInterface &, const size_t, const Wax &)> PortCallback;

//! Typedef for a block's general input message handler callback
typedef boost::function<void(const TaskInterface &, const Wax &)> BlockCallback;

//! Typedef for topological changes on the block handler callback
typedef boost::function<void(const TaskInterface &)> ChangedCallback;

struct TSBE_API BlockConfig
{
    BlockConfig(void);
    PortCallback input_callback;
    PortCallback output_callback;
    BlockCallback block_callback;
    ChangedCallback changed_callback;
};

/*!
 * A block represents a unit of computation.
 *
 * A block consumes input buffers on N input ports,
 * and produces output buffers on M output ports.
 *
 * Arbitrary messages may be accepted from upstream ports
 * and arbitrary messages may be posted to downstream ports.
 */
struct TSBE_API Block : boost::shared_ptr<ElementImpl>
{
    //! Construct a null block
    Block(void);

    //! Create a new block from config params
    Block(const BlockConfig &config);

    /*!
     * Post a message to this block to be handled by the message callback
     */
    void post_msg(const Wax &msg);
};

} //namespace tsbe

#endif /*INCLUDED_TSBE_BLOCK_HPP*/
