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

#ifndef INCLUDED_TSBE_FLOW_HPP
#define INCLUDED_TSBE_FLOW_HPP

#include <tsbe/config.hpp>
#include <tsbe/task.hpp>

namespace tsbe
{

/*!
 * And endpoint represents one half of a flow.
 * The source/destination task and respective index.
 */
struct TSBE_API FlowEndpoint
{
    FlowEndpoint(void);

    FlowEndpoint(const Task &task, const size_t index);

    //! The source or destination task
    Task task;

    //! The index for this endpoint
    size_t index;
};

TSBE_API bool operator==(const FlowEndpoint &lhs, const FlowEndpoint &rhs);

/*!
 * The configuration struct for creating a new connection
 */
struct TSBE_API FlowConfig
{
    //! Default connection config constructor
    FlowConfig(void);

    //! The source task that produces
    FlowEndpoint source;

    //! The destination task that consumes
    FlowEndpoint dest;
};

/*!
 * A flow object represents a live connection of information between
 * an output port of a source task and an input port of a destination task.
 * Creating a flow object with the config actually creates the connection.
 * Allowing all flow references to go out of scope causes a disconnect.
 */
struct TSBE_API Flow : boost::shared_ptr<FlowImpl>
{
    //! Creates a null connection
    Flow(void);

    //! Create a connection between two ports
    Flow(const FlowConfig &config);

    //! Get the source/output endpoint
    FlowEndpoint get_source(void) const;

    //! Get the destination/input endpoint
    FlowEndpoint get_dest(void) const;
};

} //namespace tsbe

#endif /*INCLUDED_TSBE_FLOW_HPP*/
