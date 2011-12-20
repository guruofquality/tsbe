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

#ifndef INCLUDED_TSBE_QUEUE_H
#define INCLUDED_TSBE_QUEUE_H

#include <tsbe/api.h>
#include <tsbe/types.h>

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * Initialize a buffer queue object
 * \param queue the buffer pool object
 * \return 0 for success -1 for failure
 */
extern TSBE_API int tsbe__queue__init(tsbe__queue_t *queue);

/*!
 * Done with the buffer queue.
 * Cleanup and deallocate internals.
 * \param queue the buffer pool object
 */
extern TSBE_API void tsbe__queue__done(tsbe__queue_t *queue);

/*!
 * Push a buffer into the back of the queue
 * \param queue the buffer pool object
 * \param buff the buffer to insert
 * \return 0 for success -1 for failure
 */
extern TSBE_API int tsbe__queue__push(
    tsbe__queue_t *queue,
    const tsbe__buffer_t *buff
);

/*!
 * Pop a buffer from the front of the queue.
 * Waits for a buffer to become available.
 * Rules for timeout:
 * - timeout > 0, actual timeout operation
 * - timeout = 0, immediate return on empty
 * - timeout < 0, block forever (not recommended)
 * \param pbool the buffer pool object
 * \param buff the buffer to load
 * \param timeout the timeout is nanoseconds
 * \return 0 for success, -1 for timeout/failure
 */
extern TSBE_API int tsbe__queue__pop(
    tsbe__queue_t *queue,
    tsbe__buffer_t *buff,
    const long long timeout_ns
);

#ifdef __cplusplus
}
#endif

#endif /*INCLUDED_TSBE_QUEUE_H*/
