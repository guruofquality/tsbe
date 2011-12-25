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

#define BOOST_DATE_TIME_POSIX_TIME_STD_CONFIG //gives boost nanosecond support

#include <tsbe/queue.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>
#include <queue>
#include <stdexcept>
#include "buffer_impl.hpp"

struct tsbe::QueueImpl{
    std::queue<tsbe::Buffer> queue;
    boost::mutex mutex;
    boost::condition_variable cond;
};

tsbe::Queue::Queue(void){
    //NOP
}

tsbe::Queue::Queue(const bool){
    this->reset(new QueueImpl());
}

void tsbe::Queue::adopt(const Buffer &buff) const{
    if (buff->owner.lock() != NULL){
        throw std::runtime_error("queue cannot adopt this buffer");
    }
    buff->owner = *this;
}

void tsbe::Queue::disown(const Buffer &buff) const{
    if (buff->owner.lock() != *this){
        throw std::runtime_error("queue cannot disown this buffer");
    }
    buff->owner.reset();
}

void tsbe::Queue::push(const Buffer &buff){
    boost::mutex::scoped_lock lock((*this)->mutex);
    (*this)->queue.push(buff);
    lock.unlock(); //unlock before notify
    (*this)->cond.notify_one();
}

tsbe::Buffer tsbe::Queue::pop(const long long timeout_ns){
    boost::mutex::scoped_lock lock((*this)->mutex);

    if (not (*this)->queue.empty()){
        //NOP
    }

    else if (timeout_ns == 0){
        Buffer null_buff;
        return null_buff;
    }

    else if (timeout_ns < 0){
        while ((*this)->queue.empty()){
            (*this)->cond.wait(lock);
        }
    }

    else if (timeout_ns > 0){
        while ((*this)->queue.empty()){
            const boost::posix_time::nanoseconds to(timeout_ns);
            //const boost::posix_time::microseconds to(timeout_ns / 1000);
            if (not (*this)->cond.timed_wait(lock, to)){
                Buffer null_buff;
                return null_buff;
            }
        }
    }

    //now, really pop the buffer
    Buffer buff = (*this)->queue.front();
    (*this)->queue.pop();
    return buff;
}
