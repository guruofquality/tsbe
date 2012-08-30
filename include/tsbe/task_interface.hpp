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

#ifndef INCLUDED_TSBE_TASK_INTERFACE_HPP
#define INCLUDED_TSBE_TASK_INTERFACE_HPP

#include <tsbe/config.hpp>
#include <tsbe/wax.hpp>

namespace tsbe
{

/*!
 * A task interface is passed into the task callback of a block.
 * Task interfaces are created by the system and user-contructed.
 */
struct TSBE_API TaskInterface : boost::shared_ptr<TaskInterfaceImpl>
{

    //! Creates a null task interface
    TaskInterface(void);

    //! Get the number of input ports (buffer consumption by task)
    size_t get_num_inputs(void) const;

    //! Get the number of output ports (buffer production by task)
    size_t get_num_outputs(void) const;

    /*!
     * Send a message to all downstream blocks on this output port.
     * \param index the output port index
     */
    void post_downstream(const size_t index, const Wax &msg) const;

    /*!
     * Send a message to all upstream blocks on this input port.
     * \param index the input port index
     */
    void post_upstream(const size_t index, const Wax &msg) const;

};

} //namespace tsbe

#endif /*INCLUDED_TSBE_TASK_INTERFACE_HPP*/
