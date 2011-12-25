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

#include <tsbe/buffer.hpp>
#include <tsbe/queue.hpp>
#include "buffer_impl.hpp"

tsbe::BufferImpl::~BufferImpl(void){
    if (this->config.callback){
        this->config.callback();
    }
}

tsbe::Buffer::Buffer(void){
    //NOP
}

tsbe::Buffer::Buffer(const BufferConfig &config){
    this->reset(new BufferImpl());
    (*this)->config = config;
    (*this)->length = config.length; //init actual length to full
    AO_store(&(*this)->ref_count, 1);
    AO_store(&(*this)->reader_count, 1);
}

void tsbe::intrusive_ptr_add_ref(BufferImpl *p){
    AO_fetch_and_add1(&p->ref_count);
}

void tsbe::intrusive_ptr_release(BufferImpl *p){
    if (AO_fetch_and_sub1(&p->ref_count) == 1){

        Queue owner;
        owner.reset(p->owner.lock(), p->owner.lock().get()); //loads owner from weak ptr

        //the buffer has a valid owner, return it to the owner
        if (owner != NULL){
            p->length = p->config.length; //reset length
            AO_store(&p->reader_count, 1); //when popped, this buffer has one reader

            Buffer buff;
            buff.reset(p);
            owner.push(buff);
        }

        //otherwise delete the buffer
        else{
            delete p;
        }
    }
}

void tsbe::Buffer::done_reading(const size_t new_readers){
    AO_fetch_and_add(&(*this)->reader_count, int(new_readers) - 1);
}

int tsbe::Buffer::get_mode_flags(void) const{
    //one reader, returns the flags as they are
    if (AO_load(&(*this)->reader_count) == 1){
        return (*this)->config.mode_flags;
    }

    //multiple readers, suppress write flag
    return (*this)->config.mode_flags & ~BUFFER_MODE_FLAG_WR;
}

int tsbe::Buffer::get_node_affinity(void) const{
    return (*this)->config.node_affinity;
}

void *tsbe::Buffer::get_memory(void) const{
    return (*this)->config.memory;
}

size_t tsbe::Buffer::get_length(void) const{
    return (*this)->length;
}

void tsbe::Buffer::set_length(const size_t length){
    (*this)->length = length;
}
