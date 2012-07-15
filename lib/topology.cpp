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

#include "element_impl.hpp"
#include <tsbe/topology.hpp>

using namespace tsbe;

Connection::Connection(void)
{
    src_port = 0;
    sink_port = 0;
}

bool tsbe::operator==(const Connection &lhs, const Connection &rhs)
{
    return (
        lhs.src == rhs.src and lhs.src_port == rhs.src_port and
        lhs.sink == rhs.sink and lhs.sink_port == rhs.sink_port
    );
}

void Topology::add_topology(const Topology &topology)
{
    (*this)->topologies.push_back(topology);
}

void Topology::connect(const Connection &connection)
{
    (*this)->connections.push_back(connection);
}

void Topology::remove_topology(const Topology &topology)
{
    if (not remove_one((*this)->topologies, topology))
    {
        throw std::runtime_error("Topology::remove_topology could not find topology");
    }
}

void Topology::disconnect(const Connection &connection)
{
    if (not remove_one((*this)->connections, connection))
    {
        throw std::runtime_error("Topology::disconnect could not find connection");
    }
}
