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

#ifndef INCLUDED_TSBE_TYPES_IMPL_H
#define INCLUDED_TSBE_TYPES_IMPL_H

#include <tsbe/types.h>
#include <tsbe/buffer.h>
#include "atomic_impl.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct{
    tbse__buffer__config_t config;
    size_t length;
    AO_t count;
} tsbe_impl__buffer_t;

#define COMPILE_TIME_ASSERT(pred)       \
    switch(0){case 0:case (pred):;}

static inline void compile_time_assertions(void){
    COMPILE_TIME_ASSERT(sizeof(tsbe_impl__buffer_t) <= sizeof(tsbe__buffer_t));
}

#ifdef __cplusplus
}
#endif


#endif /*INCLUDED_TSBE_TYPES_IMPL_H*/
