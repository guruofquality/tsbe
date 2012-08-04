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

#include "block_impl.hpp"

using namespace tsbe;

BlockConfig::BlockConfig(void)
{
    //NOP
}

Block::Block(void)
{
    //NOP
}

Block::Block(const BlockConfig &config)
{
    this->reset(new ElementImpl());
    (*this)->block = true;
    BlockActor::Parameters params;
    params.config = config;
    (*this)->actor = (*this)->framework.CreateActor<BlockActor>(params);
}

void Block::set_output_port_allocator(const size_t index, const PortAllocator &allocator)
{
    BlockAllocatorMessage message;
    message.alloc = allocator;
    message.index = index;
    (*this)->actor.Push(message, Theron::Address());
}
