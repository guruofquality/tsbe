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

#ifndef INCLUDED_TSBE_EXECUTOR_HPP
#define INCLUDED_TSBE_EXECUTOR_HPP

#include <tsbe/config.hpp>
#include <tsbe/topology.hpp>
#include <tsbe/wax.hpp>

namespace tsbe
{

struct TSBE_API ExecutorConfig
{
    ExecutorConfig(void);

    Topology topology;
};

struct TSBE_API Executor : boost::shared_ptr<ExecutorImpl>
{
    //! Create a null Executor
    Executor(void);

    Executor(const ExecutorConfig &config);

    /*!
     * Commit any topological changes to the design.
     * Call this method after calling changing connections.
     * This will add/remove any connections from the previous state,
     * and send an update topology message to any changed blocks.
     */
    void commit(void);

    /*!
     * Post a message to all blocks in the topology.
     */
    void post_msg(const Wax &msg);
};

} //namespace tsbe

#endif /*INCLUDED_TSBE_EXECUTOR_HPP*/
