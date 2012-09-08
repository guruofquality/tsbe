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

#ifndef INCLUDED_LIBTSBE_COMMON_IMPL_HPP
#define INCLUDED_LIBTSBE_COMMON_IMPL_HPP

#include <Theron/Framework.h>
#include <Theron/Actor.h>

namespace tsbe
{

template <class ActorPtr, class ValueType>
inline bool ActorSend(const ActorPtr &actor, const ValueType &value, const Theron::Address &address)
{
    return actor->GetFramework().Send(value, address, actor->GetAddress());
}

} //namespace tsbe

#endif /*INCLUDED_LIBTSBE_COMMON_IMPL_HPP*/
