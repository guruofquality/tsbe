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

#include <boost/test/unit_test.hpp>
#include <tsbe/executor.hpp>
#include <tsbe/topology.hpp>
#include <tsbe/block.hpp>
#include <iostream>

BOOST_AUTO_TEST_CASE(test_make_executor)
{
    tsbe::ExecutorConfig config;
    tsbe::Executor executor;
    BOOST_CHECK(not executor);
    executor = tsbe::Executor(config);
    BOOST_CHECK(executor);
}

BOOST_AUTO_TEST_CASE(test_make_topology)
{
    tsbe::TopologyConfig config;
    tsbe::Topology topology;
    BOOST_CHECK(not topology);
    topology = tsbe::Topology(config);
    BOOST_CHECK(topology);
}

BOOST_AUTO_TEST_CASE(test_make_block)
{
    tsbe::BlockConfig config;
    tsbe::Block block;
    BOOST_CHECK(not block);
    block = tsbe::Block(config);
    BOOST_CHECK(block);
}

BOOST_AUTO_TEST_CASE(test_simple_connections)
{
    tsbe::BlockConfig block_config;
    tsbe::Block src(block_config);
    tsbe::Block sink(block_config);
    tsbe::Connection connection(tsbe::Port(src, 0), tsbe::Port(sink, 0));

    tsbe::TopologyConfig topology_config;
    tsbe::Topology topology(topology_config);
    topology.connect(connection);

    tsbe::ExecutorConfig executor_config;
    executor_config.topology = topology;
    tsbe::Executor executor(executor_config);
    executor.update();
}
