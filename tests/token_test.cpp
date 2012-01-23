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
#include <tsbe/token.hpp>

BOOST_AUTO_TEST_CASE(token_test0){
    tsbe::Token tok;

    BOOST_CHECK(not tok);
}

/***********************************************************************
 * the token test 1
 **********************************************************************/
static size_t callback1_count = 0;

static void callback1(void)
{
    callback1_count++;
}

BOOST_AUTO_TEST_CASE(token_test1){
    tsbe::TokenConfig config;
    config.callback = &callback1;
    tsbe::Token tok(config);

    BOOST_CHECK(tok);
    BOOST_CHECK_EQUAL(tok.get_ref_count(), 0);
    BOOST_CHECK_EQUAL(callback1_count, 0);
}

/***********************************************************************
 * the token test 2
 **********************************************************************/
static size_t callback2_count = 0;

static void callback2(void)
{
    callback2_count++;
}

BOOST_AUTO_TEST_CASE(token_test2){
    tsbe::TokenConfig config;
    config.callback = &callback2;
    tsbe::Token tok(config);

    {
        tsbe::TokenRef ref = tok.get_ref();
        BOOST_CHECK_EQUAL(tok.get_ref_count(), 1);
    }
    BOOST_CHECK_EQUAL(tok.get_ref_count(), 0);
    BOOST_CHECK_EQUAL(callback2_count, 1);

    {
        tsbe::TokenRef ref0 = tok.get_ref();
        tsbe::TokenRef ref1 = tok.get_ref();
        BOOST_CHECK_EQUAL(tok.get_ref_count(), 2);
    }
    BOOST_CHECK_EQUAL(tok.get_ref_count(), 0);
    BOOST_CHECK_EQUAL(callback2_count, 2);

    {
        tsbe::TokenRef ref0 = tok.get_ref();
        tsbe::TokenRef ref1 = ref0;
        BOOST_CHECK_EQUAL(tok.get_ref_count(), 2);
    }
    BOOST_CHECK_EQUAL(tok.get_ref_count(), 0);
    BOOST_CHECK_EQUAL(callback2_count, 3);
}

/***********************************************************************
 * the token test 3
 **********************************************************************/
static size_t callback3_count = 0;

static void callback3(void)
{
    callback3_count++;
}

BOOST_AUTO_TEST_CASE(token_test3){
    tsbe::TokenConfig config;
    config.callback = &callback3;
    tsbe::Token tok(config);

    {
        tsbe::TokenRef ref = tok.get_ref();
        BOOST_CHECK_EQUAL(tok.get_ref_count(), 1);
    }
    BOOST_CHECK_EQUAL(tok.get_ref_count(), 0);
    BOOST_CHECK_EQUAL(callback3_count, 1);

    {
        tsbe::TokenRef ref = tok.get_ref();
        BOOST_CHECK_EQUAL(tok.get_ref_count(), 1);
        tok.reset(); //kill token
    }
    //callback will not execute with dead token
    BOOST_CHECK_EQUAL(callback3_count, 1);

}
