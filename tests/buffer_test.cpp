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
#include <tsbe/buffer.hpp>
#include <boost/bind.hpp>
#include <iostream>

BOOST_AUTO_TEST_CASE(test_simple_buffer)
{
    tsbe::Buffer b0;
    BOOST_CHECK(not b0);

    tsbe::BufferConfig c1;
    c1.length = 100;
    tsbe::Buffer b1(c1);
    BOOST_CHECK(b1);
    BOOST_CHECK_EQUAL(b1.get_length(), 100);
    BOOST_CHECK(b1.get_memory() != NULL);
}

static tsbe::Buffer b2;

static void b2_deleter(tsbe::Buffer &b)
{
    b2 = b;
}

BOOST_AUTO_TEST_CASE(test_token_buffer)
{
    boost::shared_ptr<tsbe::BufferDeleter> token(new tsbe::BufferDeleter(boost::bind(&b2_deleter, _1)));

    tsbe::BufferConfig c2;
    c2.length = 256;
    c2.token = token;

    //default b2 empty
    BOOST_CHECK(not b2);

    //make a buffer and let the token get called
    {
        tsbe::Buffer b2_tmp(c2);
        BOOST_CHECK(b2_tmp);
    }

    //b2 tmp deleted, token called
    BOOST_CHECK(b2);

    //reset b2, it will come right back
    b2.reset();
    BOOST_CHECK(b2);

    //kill the token, it wont come back
    token.reset();
    b2.reset();
    BOOST_CHECK(not b2);
}
