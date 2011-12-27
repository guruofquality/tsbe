//
// Copyright 2011 Josh Blum
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

#ifndef INCLUDED_TSBE_ATOMIC_IMPL_H
#define INCLUDED_TSBE_ATOMIC_IMPL_H

//#define TSBE_USE_ATOMIC_OPS //yes, use atomic ops
#define TSBE_USR_BOOST_ATOMIC //use the boost implementation

//http://www.hpl.hp.com/research/linux/atomic_ops/README.txt
#ifdef TSBE_USE_ATOMIC_OPS

#  define AO_ASSUME_WINDOWS98 //assume modern os
#  define AO_USE_PENTIUM4_INSTRS //assume modern cpu
#  include <atomic_ops.h>

#elif defined(TSBE_USR_BOOST_ATOMIC)

#include <boost/interprocess/detail/atomic.hpp>

typedef boost::uint32_t AO_t;

inline AO_t AO_load(volatile AO_t * addr){
    return boost::interprocess::detail::atomic_read32(addr);
}

inline void AO_store(volatile AO_t * addr, AO_t new_val){
    return boost::interprocess::detail::atomic_write32(addr, new_val);
}

inline AO_t AO_fetch_and_add(volatile AO_t *addr, AO_t incr){
    return boost::interprocess::detail::atomic_add32(addr, incr);
}

inline AO_t AO_fetch_and_add1(volatile AO_t *addr){
    return boost::interprocess::detail::atomic_inc32(addr);
}

inline AO_t AO_fetch_and_sub1(volatile AO_t *addr){
    return boost::interprocess::detail::atomic_dec32(addr);
}

inline int AO_compare_and_swap(volatile AO_t * addr, AO_t old_val, AO_t new_val){
    return boost::interprocess::detail::atomic_cas32(addr, new_val, old_val);
}

//not using atomic ops? define these below for your hardware
#else

typedef uint32_t AO_t;
//typedef uint32_t AO_TS_VAL_t;
//void AO_nop();
AO_t AO_load(volatile AO_t * addr);
void AO_store(volatile AO_t * addr, AO_t new_val);
AO_t AO_fetch_and_add(volatile AO_t *addr, AO_t incr);
AO_t AO_fetch_and_add1(volatile AO_t *addr);
AO_t AO_fetch_and_sub1(volatile AO_t *addr);
//void AO_or(volatile AO_t *addr, AO_t incr);
int AO_compare_and_swap(volatile AO_t * addr, AO_t old_val, AO_t new_val);
//AO_TS_VAL_t AO_test_and_set(volatile AO_TS_t * addr);

#endif

#endif /*INCLUDED_TSBE_ATOMIC_IMPL_H*/
