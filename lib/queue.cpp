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

#include <tsbe/queue.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>
#include <queue>
#include "buffer_impl.hpp"

struct tsbe::Queue::Impl{
    std::queue<Buffer> queue;
    boost::mutex mutex;
    boost::condition_variable cond;
};

tsbe::Queue tsbe::Queue::make(void){
    Queue queue;
    queue._impl = boost::shared_ptr<tsbe::Queue::Impl>(new tsbe::Queue::Impl());
    return queue;
}

bool tsbe::Queue::is_null(void) const{
    return _impl.get() == NULL;
}

void tsbe::Queue::adopt(const Buffer &buff) const{
    buff._impl->owner = *this;
}

void tsbe::Queue::push(const Buffer &buff){
    boost::mutex::scoped_lock lock(_impl->mutex);
    _impl->queue.push(buff);
    lock.unlock(); //unlock before notify
    _impl->cond.notify_one();
}

void tsbe::Queue::pop(Buffer &buff, const long long timeout_ns){
    boost::mutex::scoped_lock lock(_impl->mutex);

    if (_impl->queue.empty()){
        //NOP
    }

    else if (timeout_ns == 0){
        Buffer null_buff;
        buff = null_buff;
        return;
    }

    else if (timeout_ns < 0){
        while (_impl->queue.empty()){
            _impl->cond.wait(lock);
        }
    }

    else if (timeout_ns > 0){
        while (_impl->queue.empty()){
            if (not _impl->cond.timed_wait(lock, boost::posix_time::microseconds(timeout_ns * 1000))){
                Buffer null_buff;
                buff = null_buff;
                return;
            }
        }
    }

    //now, really pop the buffer
    buff = _impl->queue.front();
    _impl->queue.pop();
}
