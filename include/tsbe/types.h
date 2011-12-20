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

#ifndef INCLUDED_TSBE_TYPES_H
#define INCLUDED_TSBE_TYPES_H

#include <stddef.h> //size_t

#define __SIZEOF_TSBE__BUFFER_T 64
#define __SIZEOF_TSBE__QUEUE_T 8

#ifdef __cplusplus
extern "C" {
#endif

typedef union{
    char __size[__SIZEOF_TSBE__BUFFER_T];
    long int __align;
} tsbe__buffer_t;

typedef union{
    char __size[__SIZEOF_TSBE__QUEUE_T];
    long int __align;
} tsbe__queue_t;

#ifdef __cplusplus
}
#endif


#endif /*INCLUDED_TSBE_TYPES_H*/
