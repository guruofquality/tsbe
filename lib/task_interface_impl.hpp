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

#ifndef INCLUDED_LIBTSBE_TASK_INTERFACE_IMPL_HPP
#define INCLUDED_LIBTSBE_TASK_INTERFACE_IMPL_HPP

#include <tsbe/task_interface.hpp>
#include <tsbe/port.hpp>
#include <tsbe/block.hpp>
#include <queue>
#include <vector>

namespace tsbe
{

struct TaskInterfaceImpl
{
    TaskInterfaceImpl(void)
    {
        //NOP
    }

    ~TaskInterfaceImpl(void)
    {
        inputs.clear();
        outputs.clear();
    }

    std::vector<std::vector<Port> > inputs;
    std::vector<std::vector<Port> > outputs;
};

} //namespace tsbe

#endif /*INCLUDED_LIBTSBE_TASK_INTERFACE_IMPL_HPP*/
