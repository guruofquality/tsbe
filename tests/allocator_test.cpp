//
// Copyright 2011 Josh Blum
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
#include <tsbe/allocator.hpp>
#include <iostream>

BOOST_AUTO_TEST_CASE(allocator_test0){
    tsbe::Allocator allocator;

    BOOST_CHECK(allocator.get() == NULL);
}

BOOST_AUTO_TEST_CASE(allocator_test1){
    tsbe::AllocatorConfig config;
    config.length = 1024;
    config.node_affinity = 0;
    config.default_num_buffers = 1;
    config.maximum_num_buffers = 2;
    tsbe::Allocator allocator(config);

    BOOST_CHECK(allocator.get() != NULL);

    //get a bunch of buffers once
    {
        tsbe::Buffer buff0 = allocator.get_buffer(0);
        tsbe::Buffer buff1 = allocator.get_buffer(0);
        tsbe::Buffer buff2 = allocator.get_buffer(0);

        BOOST_CHECK(buff0.get() != NULL);
        BOOST_CHECK(buff1.get() != NULL);
        BOOST_CHECK(buff2.get() == NULL);
    }

    //they go out of scope back to the allocator
    //so this should work again
    {
        tsbe::Buffer buff0 = allocator.get_buffer(0);
        tsbe::Buffer buff1 = allocator.get_buffer(0);
        tsbe::Buffer buff2 = allocator.get_buffer(0);

        BOOST_CHECK(buff0.get() != NULL);
        BOOST_CHECK(buff1.get() != NULL);
        BOOST_CHECK(buff2.get() == NULL);
    }
}
