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

#ifndef INCLUDED_TSBE_TOKEN_HPP
#define INCLUDED_TSBE_TOKEN_HPP

#include <tsbe/config.hpp>
#include <boost/function.hpp>
#include <boost/intrusive_ptr.hpp>
#include <boost/shared_ptr.hpp>

namespace tsbe{

struct TokenRefImpl;
struct TokenImpl;
TSBE_API void intrusive_ptr_add_ref(TokenRefImpl *);
TSBE_API void intrusive_ptr_release(TokenRefImpl *);

/*!
 * The typedef for the token callback.
 * Notice that the function takes no arguments.
 * Bind arguments into the function with boost::bind.
 */
typedef boost::function<void(void)> TokenCB;

//! A configuration struct for creating a token
struct TSBE_API TokenConfig
{
    //! bound callback for a token
    TokenCB callback;
};

/*!
 * A token reference is a smart reference counted object.
 * The callback runs when the reference count goes to zero.
 */
struct TSBE_API TokenRef:
    boost::intrusive_ptr<TokenRefImpl>
{
    //! Create an empty token reference
    TokenRef(void);
};

/*!
 * A token is a reference counted object.
 * When the reference count goes to zero,
 * the token callback is executed.
 */
struct TSBE_API Token:
    boost::shared_ptr<TokenImpl>
{
    //! Create an empty token
    Token(void);

    //! Create a new token from config
    Token(const TokenConfig &config);

    //! Get the token reference
    TokenRef get_ref(void) const;

    //! Get the reference count
    size_t get_ref_count(void) const;
};

} //namespace tsbe

#endif /*INCLUDED_TSBE_TOKEN_HPP*/
