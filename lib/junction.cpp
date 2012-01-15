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

#define BOOST_DATE_TIME_POSIX_TIME_STD_CONFIG //gives boost nanosecond support

#include <tsbe/junction.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>
#include <boost/circular_buffer.hpp>
#include <stdexcept>
#include "buffer_impl.hpp"

struct tsbe::JunctionImpl{
    typedef boost::circular_buffer<tsbe::Buffer> queue_type;
    std::vector<queue_type> queues;
    boost::mutex mutex;
    boost::condition_variable cond;
};

tsbe::Junction::Junction(void){
    //NOP
}

tsbe::Junction::Junction(const JunctionConfig &config){
    this->reset(new JunctionImpl());
    (*this)->queues.resize(config.num_outputs);
}

void tsbe::Junction::adopt(const Buffer &buff) const{
    if (buff->owner.lock() != NULL){
        throw std::runtime_error("junction cannot adopt this buffer");
    }
    buff->owner = *this;
}

void tsbe::Junction::disown(const Buffer &buff) const{
    if (buff->owner.lock() != *this){
        throw std::runtime_error("junction cannot disown this buffer");
    }
    buff->owner.reset();
}

void tsbe::Junction::push(const Buffer &buff){
    boost::mutex::scoped_lock lock((*this)->mutex);

    buff->update_readers((*this)->queues.size());

    for (size_t i = 0; i < (*this)->queues.size(); i++){
        if ((*this)->queues[i].full()){
            const size_t new_size = (*this)->queues[i].size()*2;
            (*this)->queues[i].resize(std::max<size_t>(new_size, 2));
        }
        (*this)->queues[i].push_back(buff);
    }

    lock.unlock(); //unlock before notify
    (*this)->cond.notify_all();
}

//! Check the condition and return a suitable buffer if possible
static inline tsbe::Buffer check_condition(tsbe::JunctionImpl::queue_type &queue, const size_t num_bytes){
    //empty queue, cant do anything, null buffer
    if (queue.empty()){
        tsbe::Buffer null_buff;
        return null_buff;
    }

    //sufficient space? return front
    if (queue.front().get_length() >= num_bytes){
        return queue.front();
    }

    //obviously there is no more contiguous space, null
    if (queue.size() == 1){
        tsbe::Buffer null_buff;
        return null_buff;
    }

    //attempt to make a meta-buffer from contiguous sections
    tsbe::BufferConfig config;
    config.memory = queue.front().get_memory();
    config.length = num_bytes;
    config.mode_flags = queue.front()->config.mode_flags;
    config.node_affinity = queue.front()->config.node_affinity;
    tsbe::Buffer buff(config); //TODO get this from pre-allocated

    size_t num_bytes_accum = 0;
    for (size_t i = 0; i < queue.size(); i++){
        if (size_t(queue[i].get_memory()) == size_t(buff.get_memory()) + num_bytes_accum){
            buff->sub_buffs.push_back(queue[i]);
            num_bytes_accum += queue[i].get_length();
            if (num_bytes_accum >= num_bytes){
                return buff;
            }
        }
        else break; //not contiguous
    }

    //failure, return null buff
    tsbe::Buffer null_buff;
    return null_buff;
}

tsbe::Buffer tsbe::Junction::front(const size_t index, const size_t num_bytes, const long long timeout_ns){
    boost::mutex::scoped_lock lock((*this)->mutex);

    if (timeout_ns == 0){
        return check_condition((*this)->queues[index], num_bytes);
    }

    else if (timeout_ns < 0){
        while (true){
            tsbe::Buffer buff = check_condition((*this)->queues[index], num_bytes);
            if (buff) return buff;
            (*this)->cond.wait(lock);
        }
    }

    else if (timeout_ns > 0){
        const boost::posix_time::nanoseconds to(timeout_ns);
        boost::system_time exit_time = boost::get_system_time() + to;
        while (true){
            if (not (*this)->cond.timed_wait(lock, exit_time)) break;
            tsbe::Buffer buff = check_condition((*this)->queues[index], num_bytes);
            if (buff) return buff;
        }
    }

    //null buffer
    Buffer null_buff;
    return null_buff;
}

void tsbe::Junction::pop(const size_t index, const size_t num_bytes){
    boost::mutex::scoped_lock lock((*this)->mutex);

    JunctionImpl::queue_type &queue = (*this)->queues[index];

    //decrease the readers by the number of queues
    //TODO and do sub buffers AO_fetch_and_add(

    //free buffers in the queue up until num_bytes
    //if we end in the middle of a buffer, save it in a meta-buffer
    size_t num_bytes_accum = 0;
    while (not queue.empty()){
        num_bytes_accum += queue.front().get_length();
        if (num_bytes_accum <= num_bytes){
            queue.front().reset();
            queue.pop_front();
        }
        if (num_bytes_accum == num_bytes) return; //done normally
        if (num_bytes_accum < num_bytes) continue; //keep going
        if (num_bytes_accum > num_bytes){ //do a meta-buffer
            tsbe::BufferConfig config;
            config.length = num_bytes_accum - num_bytes;
            config.memory = (void *)(size_t(queue.front().get_memory()) + queue.front().get_length() - config.length);
            config.mode_flags = queue.front()->config.mode_flags;
            config.node_affinity = queue.front()->config.node_affinity;
            tsbe::Buffer buff(config); //TODO get this from pre-allocated

            buff->sub_buffs.push_back(queue.front());
            queue.front() = buff; //swap ref

            return;
        }
    }

    throw std::runtime_error("junction cannot pop this many bytes");
}
