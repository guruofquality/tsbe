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

#include <tsbe_impl/thread_pool_impl.hpp>
#include <tsbe_impl/common_impl.hpp>
#include <tsbe_impl/block_impl.hpp>

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
    (*this)->thread_pool = ThreadPool::get_active();
    (*this)->actor = boost::shared_ptr<Actor>(new BlockActor((*this)->thread_pool->framework, config));
}

void Block::post_msg(const Wax &msg)
{
    BlockPostMessage message;
    message.msg = msg;
    (*this)->actor->Push(message, Theron::Address());
}
