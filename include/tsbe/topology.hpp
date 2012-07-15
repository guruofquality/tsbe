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

#ifndef INCLUDED_TSBE_TOPOLOGY_HPP
#define INCLUDED_TSBE_TOPOLOGY_HPP

#include <tsbe/config.hpp>
#include <boost/shared_ptr.hpp>

namespace tsbe
{

struct TSBE_API TopologyConfig
{
    TopologyConfig(void);
};

/*!
 * Topology is an interconnection of elements,
 * which includes: child topologies and blocks.
 * A topology describes a data flow for blocks.
 *
 * Topology provides API control for fine grained control of:
 *   - group processing structure
 *   - affinity of processing structure
 *   - processing unit chunk size
 */
struct TSBE_API Topology : boost::shared_ptr<ElementImpl>
{
    //! Create a null Topology
    Topology(void);

    Topology(const TopologyConfig &config);

    //! Get access to this topology as an Element for connect
    Element self(void);

    /*!
     * Add a child topology to this parent.
     * Normally the hierarchy is implied by connect:
     * This routine is reserved for a topology without IO.
     */
    void add_child(const Topology &src);

    /*!
     * Connect the outport port of an element to the input port of an element.
     * The element may be a block, a topology, or this object via self().
     */
    void connect(
        const Element &src, const size_t src_port,
        const Element &sink, const size_t sink_port
    );

    //! Remove a child topology from this parent.
    void remove_child(const Topology &src);

    //! Disconnect an output port from an input port
    void disconnect(
        const Element &src, const size_t src_port,
        const Element &sink, const size_t sink_port
    );
};

} //namespace tsbe

#endif /*INCLUDED_TSBE_TOPOLOGY_HPP*/
