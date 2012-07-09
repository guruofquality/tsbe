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
#include <tsbe/task.hpp>
#include <boost/shared_ptr.hpp>

namespace tsbe
{

struct TopologyImpl;

struct TSBE_API TopologyConfig
{
    //empty
};

struct TSBE_API Topology : boost::shared_ptr<TopologyImpl>
{
    Topology(void);

    Topology(const TopologyConfig &config);

    void activate(void);

    void deactivate(void);

    void connect(
        const Task &source_task, const size_t source_index,
        const Task &sink_task, const size_t sink_index
    );

    void disconnect(
        const Task &source_task, const size_t source_index,
        const Task &sink_task, const size_t sink_index
    );
};

} //namespace tsbe

#endif /*INCLUDED_TSBE_TOPOLOGY_HPP*/
