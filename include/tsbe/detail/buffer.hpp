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

#ifndef INCLUDED_TSBE_DETAIL_BUFFER_HPP
#define INCLUDED_TSBE_DETAIL_BUFFER_HPP

#include <boost/asio/buffer.hpp>

namespace tsbe{

    inline ConstBuffer::ConstBuffer(void)
    {
        //NOP
    }

    inline ConstBuffer::ConstBuffer(const void *mem, const size_t len):
        boost::asio::const_buffer(boost::asio::buffer(mem, len))
    {
        //NOP
    }

    inline size_t ConstBuffer::size(void) const
    {
        return boost::asio::buffer_size(*this);
    }

    template <typename Type>
    inline const Type *ConstBuffer::cast(void) const
    {
        return boost::asio::buffer_cast<Type>(*this);
    }

    inline MutableBuffer::MutableBuffer(void)
    {
        //NOP
    }

    inline MutableBuffer::MutableBuffer(void *mem, const size_t len):
        boost::asio::mutable_buffer(boost::asio::buffer(mem, len))
    {
        //NOP
    }

    inline size_t MutableBuffer::size(void) const
    {
        return boost::asio::buffer_size(*this);
    }

    template <typename Type>
    inline Type *MutableBuffer::cast(void) const
    {
        return boost::asio::buffer_cast<Type>(*this);
    }

} //namespace tsbe

#endif /*INCLUDED_TSBE_DETAIL_BUFFER_HPP*/
