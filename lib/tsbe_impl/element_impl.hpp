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

#ifndef INCLUDED_LIBTSBE_ELEMENT_IMPL_HPP
#define INCLUDED_LIBTSBE_ELEMENT_IMPL_HPP

#include <tsbe/thread_pool.hpp>
#include <tsbe_impl/common_impl.hpp>
#include <vector>
#include <queue>
#include <iostream>

namespace tsbe
{

//! ElementImpl is both a topology and a block to allow interconnection
struct ElementImpl
{
    ElementImpl(void)
    {
        //NOP
    }

    ~ElementImpl(void)
    {
        this->actor.reset();
    }

    bool block;

    bool is_block(void)
    {
        return block;
    }

    boost::shared_ptr<Actor> actor;
    boost::shared_ptr<Theron::Framework> framework;
    ThreadPool thread_pool;
};

} //namespace tsbe

#endif /*INCLUDED_LIBTSBE_ELEMENT_IMPL_HPP*/
