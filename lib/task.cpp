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

#include <tsbe/task.hpp>
#include <boost/thread/mutex.hpp>

struct tsbe::TaskImpl{
    TaskConfig config;
    boost::mutex mutex;
};

tsbe::Task::Task(void){
    //NOP
}

tsbe::Task::Task(const TaskConfig &config){
    this->reset(new TaskImpl());
    (*this)->config = config;
}

tsbe::Buffer tsbe::Task::pop_buffer(const size_t index, const long long timeout_ns){
    return (*this)->config.inputs[index].pop(timeout_ns);
}

void tsbe::Task::push_buffer(const size_t index, const Buffer &buff){
    (*this)->config.outputs[index].push(buff);
}

bool tsbe::Task::run_one(void){
    if (not (*this)->mutex.try_lock()) return false;
    try{
        (*this)->config.callback(*this);
    }
    catch(...){
        //TODO maybe we dont ignore exceptions
    }
    (*this)->mutex.unlock();
    return true;
}
