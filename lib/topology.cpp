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

#include <tsbe/topology.hpp>
#include "tsbe_impl.hpp"
#include <vector>

using namespace tsbe;

struct tsbe::TopologyImpl
{
    TopologyConfig config;
    std::vector<Task> tasks;
};

Topology::Topology(void)
{
    //NOP
}

Topology::Topology(const TopologyConfig &config)
{
    this->reset(new TopologyImpl());
    (*this)->config = config;
}

void Topology::activate(void)
{
    
}

void Topology::deactivate(void)
{
    
}

void Topology::connect(
    const Task &source_task, const size_t source_index,
    const Task &sink_task, const size_t sink_index
){
    
}

void Topology::disconnect(
    const Task &source_task, const size_t source_index,
    const Task &sink_task, const size_t sink_index
){
    
}
