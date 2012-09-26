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
#include <tsbe/connection.hpp>

namespace tsbe
{

struct TSBE_API TopologyConfig
{
    TopologyConfig(void);
};

/*!
 * Topology is an interconnection of bases,
 * which includes: child topologies and blocks.
 * A topology describes a data flow for blocks.
 *
 * Topology provides API control for fine grained control of:
 *   - group processing structure
 *   - affinity of processing structure
 *   - processing unit chunk size
 */
struct TSBE_API Topology : boost::shared_ptr<BaseImpl>
{
    //! Create a null Topology
    Topology(void);

    //! Create a new Topology object
    Topology(const TopologyConfig &config);

    //! Get access to this topology as an Base for connect
    const Base &self(void) const;

    /*!
     * Add a child topology to this parent.
     * Normally the hierarchy is implied by connect:
     * This routine is reserved for a topology without IO.
     */
    void add_topology(const Topology &topology);

    //! Remove a child topology from this parent.
    void remove_topology(const Topology &topology);

    /*!
     * Connect the outport port of an base to the input port of an base.
     * The base may be a block, a topology, or this object via self().
     */
    void connect(const Connection &connection);

    //! Disconnect an output port from an input port
    void disconnect(const Connection &connection);

    /*!
     * Reset this topology to factory state.
     * In other words, remove all connections and sub-topologies.
     */
    void clear_all(void);
};

} //namespace tsbe

#endif /*INCLUDED_TSBE_TOPOLOGY_HPP*/
