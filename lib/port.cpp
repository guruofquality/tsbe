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

#include <tsbe/port.hpp>

using namespace tsbe;

Port::Port(void)
{
    index = 0;
}

Port::Port(const Base &elem, const size_t index, Container container):
    elem(elem), index(index), container(container)
{
    //NOP
}

bool tsbe::operator==(const Port &lhs, const Port &rhs)
{
    return (lhs.elem == rhs.elem and lhs.index == rhs.index);
}
