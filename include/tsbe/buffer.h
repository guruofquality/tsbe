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

#ifndef INCLUDED_TSBE_BUFFER_H
#define INCLUDED_TSBE_BUFFER_H

#include <tsbe/api.h>
#include <tsbe/types.h>

#ifdef __cplusplus
extern "C" {
#endif

//! Possible mode flags
#define TBSE__BUFFER__MODE_FLAG_RD (1 << 0)
#define TBSE__BUFFER__MODE_FLAG_WR (1 << 1)

/*!
 * A callback type for a buffer.
 * This is used when a buffer is marked done.
 * The void* data pointer can be anything.
 * Ex: data may be the mem pointer so the callback can call free() on it
 */
typedef void(*tsbe__buffer__cb_t)(void *data);

/*!
 * The configuration struct for initializing a new buffer
 */
typedef struct{
    void *memory;
    size_t length;
    int mode_flags;
    int node_affinity;
    tsbe__buffer__cb_t done_cb;
    void *done_data;
} tbse__buffer__config_t;

/*!
 * Initialize a buffer object.
 * Set its memory, length, and callback
 * \param buff the buffer to init
 * \param config the configuration data
 */
extern TSBE_API void tsbe__buffer__init(
    tsbe__buffer_t *buff,
    const tbse__buffer__config_t *config
);

/*!
 * Mark the buffer as done.
 * Internally, this decrements the reference count.
 * When the count goes to zero, callback is invoked.
 * The num readers allows the caller to tell the buffer
 * how many extra things will also hold onto this buffer,
 * even after the current caller is done with the buffer.
 * \param buff the buffer object
 * \param num_readers extra buffer reference holders
 */
extern TSBE_API void tsbe__buffer__done(
    tsbe__buffer_t *buff,
    const size_t num_readers
);

/*!
 * Get the RW mode of the buffer.
 * Note: A writable buffer will not have WR set when it has multiple readers.
 * \param buff the buffer object
 * \return the RW mode flags
 */
extern TSBE_API int tsbe__buffer__get_mode_flags(const tsbe__buffer_t *buff);

/*!
 * Get the memory node affinity of this buffer.
 * \param buff the buffer object
 * \return the node affinity or -1 for not applicable
 */
extern TSBE_API int tsbe__buffer__get_node_affinity(const tsbe__buffer_t *buff);

//! Get the memory address of the buffer
extern TSBE_API void *tsbe__buffer__get_memory(const tsbe__buffer_t *buff);

//! Get the length in bytes of the buffer
extern TSBE_API size_t tsbe__buffer__get_length(const tsbe__buffer_t *buff);

/*!
 * Set the length in bytes of the buffer.
 * This is the number of bytes filled with useful data.
 * \param buff the buffer object
 * \param length the length in bytes
 */
extern TSBE_API void tsbe__buffer__set_length(tsbe__buffer_t *buff, const size_t length);

#ifdef __cplusplus
}
#endif

#endif /*INCLUDED_TSBE_BUFFER_H*/
