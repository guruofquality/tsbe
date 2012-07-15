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

#ifndef INCLUDED_TSBE_BLOCK_HPP
#define INCLUDED_TSBE_BLOCK_HPP

#include <tsbe/config.hpp>
#include <boost/shared_ptr.hpp>
#include <string>

namespace tsbe
{

struct TSBE_API Block : boost::shared_ptr<ElementImpl>
{
    Block(void);

    //! Get the number of input ports
    size_t get_num_inputs(void);

    //! Get the number of output ports
    size_t get_num_outputs(void);

    //! Set the associated task group, or empty for None
    void set_task_group(const std::string &group);

    //! Get the associated task group
    std::string get_task_group(void);
};

} //namespace tsbe

#endif /*INCLUDED_TSBE_BLOCK_HPP*/
