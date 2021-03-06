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
#include <tsbe/port.hpp>

namespace tsbe
{

/*!
 * A connection represents a data path between a source port and a sink port.
 */
struct TSBE_API Connection
{
    //! Create an unitialized connection
    Connection(void);

    //! Create a connection from source and sink
    Connection(const Port &src, const Port &sink);

    //! The source port (provides data)
    Port src;

    //! The sink port (accepts data)
    Port sink;
};

//! a comparison operator overload for connections
TSBE_API bool operator==(const Connection &lhs, const Connection &rhs);

} //namespace tsbe

#endif /*INCLUDED_TSBE_CONNECTION_HPP*/
