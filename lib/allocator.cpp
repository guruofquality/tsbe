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

#include <tsbe/allocator.hpp>
#include <tsbe/queue.hpp>
#include <boost/bind.hpp>
#include <numanuma.hpp>
#include "atomic_impl.h"

struct tsbe::AllocatorImpl{
    AllocatorConfig config;
    Queue queue;
    AO_t num_allocated;
    Buffer alloc_one(void);
};

static void free_numanuma_mem(numanuma::mem *memory){
    delete memory;
}

tsbe::Buffer tsbe::AllocatorImpl::alloc_one(void){
    //TODO how should we handle -1 affinity?
    //make use standard malloc, but for now just use node 0
    int node_affinity = this->config.node_affinity;
    if (node_affinity < 0) node_affinity = 0;

    //allocate the memory
    numanuma::mem *memory = numanuma::mem::make(
        this->config.node_affinity,
        this->config.length
    );

    //fill in the buffer config
    BufferConfig c;
    c.memory = memory->get();
    c.length = memory->len();
    c.mode_flags = this->config.node_affinity;
    c.node_affinity = BUFFER_MODE_FLAG_RD | BUFFER_MODE_FLAG_WR;
    c.callback = boost::bind(&free_numanuma_mem, memory);

    //make the buffer
    Buffer buff(c);
    queue.adopt(buff);
    AO_fetch_and_add1(&num_allocated);

    return buff;
}

tsbe::Allocator::Allocator(void){
    //NOP
}

tsbe::Allocator::Allocator(const AllocatorConfig &config){
    this->reset(new AllocatorImpl());
    (*this)->config = config;
    (*this)->queue = Queue(true);

    //allocate default number of buffers
    AO_store(&(*this)->num_allocated, 0);
    for (size_t i = 0; i < config.default_num_buffers; i++){
        (*this)->alloc_one();
    }
}

tsbe::Buffer tsbe::Allocator::get_buffer(const long long timeout_ns){

    //try a non-blocking pop
    Buffer buff = (*this)->queue.pop(0);
    if (buff) return buff;

    //can we allocate?
    if (AO_load(&(*this)->num_allocated) < (*this)->config.maximum_num_buffers){
        return (*this)->alloc_one();
    }

    //otherwise blocking pop
    return (*this)->queue.pop(timeout_ns);
}
