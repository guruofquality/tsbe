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
    this->set_task_group("");
}

void Block::set_task_group(const std::string &group)
{
    if (group.empty())
    {
        boost::uuids::uuid u; //initialize uuid
        (*this)->group = to_string(u);
    }
    else
    {
        (*this)->group = group;
    }
}
