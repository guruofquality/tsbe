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

#ifndef INCLUDED_TSBE_THREAD_POOL_HPP
#define INCLUDED_TSBE_THREAD_POOL_HPP

#include <tsbe/config.hpp>

namespace tsbe
{

typedef unsigned long long NUMAMask;

extern TSBE_API NUMAMask NUMA_MASK_NULL;

//! Configuration struct for a new thread pool
struct TSBE_API ThreadPoolConfig
{
    ThreadPoolConfig(void);

    //! The number of threads to spawn
    size_t num_threads;

    //! Specifies the NUMA processor nodes upon which the framework may execute.
    NUMAMask node_mask;

    //! Specifies the subset of the processors in each NUMA processor node upon which the framework may execute.
    NUMAMask processor_mask;
};

/*!
 * A thread pool represents a group of processing threads
 * which may be affinitized to a specific part of the computer.
 */
struct TSBE_API ThreadPool : boost::shared_ptr<ThreadPoolImpl>
{
    //! Creates an empty thread pool
    ThreadPool(void);

    //! Create a new thread pool from config
    ThreadPool(const ThreadPoolConfig &config);

    static ThreadPool get_active(void);

    /*!
     * Set this thread pool as the active on.
     *
     * Any blocks or topologies created after the activation
     * will operate within the context of a thread in this pool.
     *
     * Activation displaced the currently active thread pool.
     */
    void activate(void);
};

} //namespace tsbe

#endif /*INCLUDED_TSBE_THREAD_POOL_HPP*/
