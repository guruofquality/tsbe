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

#include "common_impl.hpp"
#include <boost/thread/thread.hpp>
#include <iostream>

static Theron::Framework::Parameters get_params(void)
{
    const size_t n = boost::thread::hardware_concurrency();
    std::cerr << "boost::thread::hardware_concurrency() = " << n << std::endl;
    Theron::Framework::Parameters params((n < 2)?2:n);
    return params;
}

Theron::Framework &tsbe::get_master_framework(void)
{
    static Theron::Framework framework(get_params());
    return framework;
}
