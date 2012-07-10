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
#include <tsbe/task.hpp>
#include <tsbe/connection.hpp>
#include <boost/bind.hpp>
#include <iostream>

void test_callback(tsbe::Task &task)
{
    //NOP
}

BOOST_AUTO_TEST_CASE(test_simple_connect)
{
    tsbe::TaskConfig task_config0;
    task_config0.callback = &test_callback;
    tsbe::Task task0(task_config0);

    tsbe::TaskConfig task_config1;
    task_config1.callback = &test_callback;
    tsbe::Task task1(task_config1);

    {

        tsbe::ConnectionConfig conn_config;
        conn_config.source_task = task0;
        conn_config.source_index = 0;
        conn_config.dest_task = task1;
        conn_config.dest_index = 0;
        tsbe::Connection conn(conn_config);

        BOOST_CHECK_EQUAL(task0.get_num_outputs(), 1);
        BOOST_CHECK_EQUAL(task0.get_num_inputs(), 0);
        BOOST_CHECK_EQUAL(task1.get_num_outputs(), 0);
        BOOST_CHECK_EQUAL(task1.get_num_inputs(), 1);

    }

    BOOST_CHECK_EQUAL(task0.get_num_outputs(), 0);
    BOOST_CHECK_EQUAL(task0.get_num_inputs(), 0);
    BOOST_CHECK_EQUAL(task1.get_num_outputs(), 0);
    BOOST_CHECK_EQUAL(task1.get_num_inputs(), 0);

}
