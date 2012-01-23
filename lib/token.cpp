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

#include "atomic_impl.h"
#include <tsbe/token.hpp>
#include <boost/weak_ptr.hpp>

struct tsbe::TokenImpl
{
    TokenRefImpl *impl;
    ~TokenImpl(void);
};

struct tsbe::TokenRefImpl
{
    AO_t ref_count;
    TokenCB callback;
    boost::weak_ptr<TokenImpl> owner;
};

tsbe::TokenImpl::~TokenImpl(void)
{
    if (AO_load(&impl->ref_count) == 0)
    {
        delete impl;
    }
}

void tsbe::intrusive_ptr_add_ref(TokenRefImpl *impl)
{
    AO_fetch_and_add1(&impl->ref_count);
}

void tsbe::intrusive_ptr_release(TokenRefImpl *impl)
{
    if (AO_fetch_and_sub1(&impl->ref_count) == 1)
    {
        boost::shared_ptr<TokenImpl> owner = impl->owner.lock();
        if (owner) impl->callback();
        else delete impl;
    }
}

tsbe::TokenRef::TokenRef(void){
    //NOP
}

tsbe::Token::Token(void){
    //NOP
}

tsbe::Token::Token(const TokenConfig &config)
{
    this->reset(new TokenImpl());
    (*this)->impl = new TokenRefImpl();
    AO_store(&(*this)->impl->ref_count, 0);
    (*this)->impl->callback = config.callback;
    (*this)->impl->owner = *this;
}

tsbe::TokenRef tsbe::Token::get_ref(void) const
{
    tsbe::TokenRef ref;
    ref.reset((*this)->impl);
    return ref;
}

size_t tsbe::Token::get_ref_count(void) const
{
    return AO_load(&(*this)->impl->ref_count);
}
