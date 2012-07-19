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

#include "element_impl.hpp"
#include <boost/foreach.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>

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
    this->reset(new ElementImpl(config));
}

const BitSet& Block::get_inputs_ready(void)
{
    return (*this)->inputs_ready;
}

const BitSet& Block::get_outputs_ready(void)
{
    return (*this)->outputs_ready;
}

Buffer &Block::get_input_buffer(const size_t index)
{
    //TODO throw bad index or empty
    return (*this)->input_buffer_queues[index].front();
}

Buffer &Block::get_output_buffer(const size_t index)
{
    //TODO throw bad index or empty
    return (*this)->output_buffer_queues[index].front();
}

void Block::pop_input_buffer(const size_t index)
{
    //TODO throw bad index or empty
    (*this)->input_buffer_queues[index].pop();
}

void Block::pop_output_buffer(const size_t index)
{
    //TODO throw bad index or empty
    (*this)->output_buffer_queues[index].pop();
}

void Block::send_buffer(const size_t index, const Buffer &buff)
{
    //TODO throw bad index
    BOOST_FOREACH(Port &port, (*this)->outputs[index])
    {
        //ep.task.push_input_buffer(ep.index, buff);
    }
}

size_t Block::get_num_inputs(void)
{
    return (*this)->inputs.size();
}

size_t Block::get_num_outputs(void)
{
    return (*this)->outputs.size();
}
