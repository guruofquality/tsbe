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
#include <tsbe/buffer.hpp>
#include <vector>
#include <iostream>

BOOST_AUTO_TEST_CASE(buffer_test0){
    tsbe::Buffer buff;

    BOOST_CHECK(buff.get() == NULL);
}

BOOST_AUTO_TEST_CASE(buffer_test1){
    tsbe::BufferConfig config;
    tsbe::Buffer buff(config);

    BOOST_CHECK(buff.get() != NULL);
}

BOOST_AUTO_TEST_CASE(buffer_test2){
    tsbe::BufferConfig config;
    std::vector<char> mem(100);
    config.memory = &mem.front();
    config.length = mem.size();
    tsbe::Buffer buff(config);

    BOOST_CHECK_EQUAL(buff.get_memory(), &mem.front());
    BOOST_CHECK_EQUAL(buff.get_length(), mem.size());
    buff.set_length(50);
    BOOST_CHECK_EQUAL(buff.get_length(), 50);
}

BOOST_AUTO_TEST_CASE(buffer_test3){
    tsbe::BufferConfig config;
    config.mode_flags = tsbe::BUFFER_MODE_FLAG_RD | tsbe::BUFFER_MODE_FLAG_WR;
    tsbe::Buffer buff(config);

    BOOST_CHECK_EQUAL(buff.get_mode_flags(), tsbe::BUFFER_MODE_FLAG_RD | tsbe::BUFFER_MODE_FLAG_WR);
    buff.done_reading(2); //simulate passing this onto extra readers
    BOOST_CHECK_EQUAL(buff.get_mode_flags(), tsbe::BUFFER_MODE_FLAG_RD);
}

BOOST_AUTO_TEST_CASE(buffer_test4){
    tsbe::BufferConfig config;
    config.mode_flags = tsbe::BUFFER_MODE_FLAG_RD;
    tsbe::Buffer buff(config);

    BOOST_CHECK_EQUAL(buff.get_mode_flags(), tsbe::BUFFER_MODE_FLAG_RD);
    buff.done_reading(2); //simulate passing this onto extra readers
    BOOST_CHECK_EQUAL(buff.get_mode_flags(), tsbe::BUFFER_MODE_FLAG_RD);
}
