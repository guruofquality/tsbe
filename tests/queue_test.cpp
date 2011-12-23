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
#include <tsbe/queue.hpp>
#include <iostream>

BOOST_AUTO_TEST_CASE(queue_test0){
    tsbe::Queue queue;

    BOOST_CHECK(queue.get() == NULL);
}

BOOST_AUTO_TEST_CASE(queue_test1){
    tsbe::Queue queue(true);

    BOOST_CHECK(queue.get() != NULL);
}

BOOST_AUTO_TEST_CASE(queue_test2){
    tsbe::Queue queue(true);

    //buff should go out of scope and delete into queue
    {
        tsbe::BufferConfig config;
        tsbe::Buffer buff(config);
        queue.adopt(buff);
    }

    //find the buffer in the queue
    {
        tsbe::Buffer buff;
        queue.pop(buff, 0);
        BOOST_CHECK(buff.get() != NULL);
    }

    //find the buffer in the queue, this time disown
    {
        tsbe::Buffer buff;
        queue.pop(buff, 0);
        BOOST_CHECK(buff.get() != NULL);
        queue.disown(buff);
    }

    //dont find the buffer in the queue
    {
        tsbe::Buffer buff;
        queue.pop(buff, 0);
        BOOST_CHECK(buff.get() == NULL);
    }
}
