//
// Copyright 2011-2012 Josh Blum
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

#ifndef INCLUDED_TSBE_BUFFER_IMPL_HPP
#define INCLUDED_TSBE_BUFFER_IMPL_HPP

//! the buffer impl code is common so queue can also access it

#include <tsbe/buffer.hpp>
#include "atomic_impl.h"
#include <boost/weak_ptr.hpp>
#include <vector>

struct tsbe::BufferImpl{
    ~BufferImpl(void);
    BufferConfig config;
    AO_t ref_count;
    AO_t reader_count;
    boost::weak_ptr<JunctionImpl> owner;
    std::vector<Buffer> sub_buffs;

    inline void update_readers(int incr){
        AO_fetch_and_add(&reader_count, incr);
        for (size_t i = 0; i < sub_buffs.size(); i++){
            sub_buffs[i].update_readers(incr);
        }
    }
};

#endif /*INCLUDED_TSBE_BUFFER_IMPL_HPP*/
