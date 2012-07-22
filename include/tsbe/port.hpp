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

#ifndef INCLUDED_TSBE_PORT_HPP
#define INCLUDED_TSBE_PORT_HPP

#include <tsbe/config.hpp>

namespace tsbe
{

/*!
 * A port represents a the IO port of an element.
 * The element can be a block or a topology.
 * The index can be a input or output index.
 * The interpretation of port as a source of data
 * or a sink for data is up to the caller.
 */
struct TSBE_API Port
{
    //! Create an unitialized port
    Port(void);

    //! Create a port from an element and IO index
    Port(const Element &elem, const size_t index);

    //! The element in this port
    Element elem;

    //! The IO index of this port
    size_t index;
};

//! a comparison operator overload for ports
TSBE_API bool operator==(const Port &lhs, const Port &rhs);

} //namespace tsbe

#endif /*INCLUDED_TSBE_PORT_HPP*/
