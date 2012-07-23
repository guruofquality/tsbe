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
typedef boost::function<void(const size_t, const boost::any &)> PortCallback;

//! Typedef for a block's regular task handler callback
typedef boost::function<void(const TaskInterface &)> TaskCallback;

struct TSBE_API BlockConfig
{
    BlockConfig(void);
    PortCallback port_callback;
    TaskCallback task_callback;
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
};

} //namespace tsbe

#endif /*INCLUDED_TSBE_BLOCK_HPP*/
