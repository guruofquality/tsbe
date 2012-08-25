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
#include <tsbe/block.hpp>
#include <iostream>

BOOST_AUTO_TEST_CASE(test_copy_empty_block)
{
    tsbe::Block block;
    BOOST_CHECK(not block);

    tsbe::Block block_copy0 = block;
    BOOST_CHECK(not block_copy0);

    BOOST_CHECK_EQUAL(block.get(), block_copy0.get());
}

BOOST_AUTO_TEST_CASE(test_copy_allocated_block)
{
    tsbe::BlockConfig config;
    tsbe::Block block(config);
    BOOST_CHECK(block);

    tsbe::Block block_copy0 = block;
    BOOST_CHECK(block_copy0);

    BOOST_CHECK_EQUAL(block.get(), block_copy0.get());
}

