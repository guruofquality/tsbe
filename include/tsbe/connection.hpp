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

#ifndef INCLUDED_TSBE_CONNECTION_HPP
#define INCLUDED_TSBE_CONNECTION_HPP

#include <tsbe/config.hpp>
#include <tsbe/task.hpp>
#include <boost/shared_ptr.hpp>

namespace tsbe
{

/*!
 * The configuration struct for creating a new connection
 */
struct TSBE_API ConnectionConfig
{
    //! Default connection config constructor
    ConnectionConfig(void);

    //! The source task that produces
    Task source_task;

    //! The index of source production
    size_t source_index;

    //! The destination task that consumes
    Task dest_task;

    //! The index of destination consumption
    size_t dest_index;
};

/*!
 * A connection object represents a flow of information between
 * an output port of a source task and an input port of a destination task.
 * Creating a connection object with the config actually creates the connection.
 * Allowing all connection references to go out of scope causes a disconnect.
 */
struct TSBE_API Connection : boost::shared_ptr<ConnectionImpl>
{
    //! Creates a null connection
    Connection(void);

    //! Create a connection between two ports
    Connection(const ConnectionConfig &config);

    //! Get the source/output task
    Task get_source_task(void) const;

    //! Get the source/outport port index
    size_t get_source_index(void) const;

    //! Get the destination/input task
    Task get_dest_task(void) const;

    //! Get the destination/input port index
    size_t get_dest_index(void) const;
};

} //namespace tsbe

#endif /*INCLUDED_TSBE_CONNECTION_HPP*/
