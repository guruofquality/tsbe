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
#include <tsbe/task.hpp>
#include <vector>
#include <iostream>

BOOST_AUTO_TEST_CASE(task_test0){
    tsbe::Task task;

    BOOST_CHECK(task.get() == NULL);
}

static void task_callback(tsbe::Task &task){
    //just a dummy forward task
    tsbe::Buffer buff = task.pop_buffer(0/*index*/, 0/*to*/);
    if (buff) task.push_buffer(0/*index*/, buff);
}

BOOST_AUTO_TEST_CASE(task_test1){
    //make queues
    tsbe::QueueConfig empty_queue_config;
    tsbe::Queue input_queue(empty_queue_config);
    tsbe::Queue output_queue(empty_queue_config);

    //make task
    tsbe::TaskConfig task_config;
    task_config.inputs.push_back(input_queue);
    task_config.outputs.push_back(output_queue);
    task_config.callback = &task_callback;
    tsbe::Task task(task_config);

    BOOST_CHECK(task.get() != NULL);

    //make a buffer, feed it to the input queue
    std::vector<char> mem(100);
    {
        tsbe::BufferConfig buff_config;
        buff_config.memory = &mem.front();
        buff_config.length = mem.size();
        tsbe::Buffer buff(buff_config);
        input_queue.push(buff);
    }

    //run it
    BOOST_CHECK(task.run_one());

    //pop the buffer from output, should be the same
    {
        tsbe::Buffer buff = output_queue.pop(0);
        BOOST_CHECK(buff.get() != NULL);
        BOOST_CHECK_EQUAL(buff.get_memory(), &mem.front());
        BOOST_CHECK_EQUAL(buff.get_length(), mem.size());
    }
}
