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

#ifndef INCLUDED_TSBE_DELETER_HPP
#define INCLUDED_TSBE_DELETER_HPP

#include <tsbe/config.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>

namespace tsbe
{

class DeleterImpl;

/*!
 * A deleter is a reference counted object
 * that calls a bound function upon de-ref.
 */
class TSBE_API Deleter : boost::shared_ptr<DeleterImpl>
{
    typedef boost::function<void(void)> Callback;

    //! Create an empty deleter
    Deleter(void);

    //! Create a deleter with callback
    Deleter(const Callback &cb);
};


} //namespace tsbe

#endif /*INCLUDED_TSBE_DELETER_HPP*/