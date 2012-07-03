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

#ifndef INCLUDED_TSBE_DATA_FLOW_TOPOLOGY_HPP
#define INCLUDED_TSBE_DATA_FLOW_TOPOLOGY_HPP

#include <tsbe/config.hpp>
#include <tsbe/buffer.hpp>
#include <boost/shared_ptr.hpp>
#include <string>
#include <vector>

namespace tsbe{

//policy:
// byte multiple, byte alignment

struct TaskUnit
{
    std::vector<ConstBuffer> inputs;
    std::vector<MutableBuffer> outputs;
};

struct DataFlowTopologyImpl;

/*!
 * 
 */
struct TSBE_API DataFlowTopology:
    boost::shared_ptr<DataFlowTopologyImpl>
{
    DataFlowTopology(void);

    

    //std::vector<std::string> get_nodes(void) const;

    

};

} //namespace tsbe

#endif /*INCLUDED_TSBE_DATA_FLOW_TOPOLOGY_HPP*/
