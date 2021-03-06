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

#include <tsbe/connection.hpp>

using namespace tsbe;

Connection::Connection(void)
{
    //NOP
}

Connection::Connection(const Port &src, const Port &sink):
    src(src), sink(sink)
{
    //NOP
}

bool tsbe::operator==(const Connection &lhs, const Connection &rhs)
{
    return (lhs.src == rhs.src and lhs.sink == rhs.sink);
}
