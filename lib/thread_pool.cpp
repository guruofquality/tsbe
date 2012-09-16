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

#include <tsbe_impl/thread_pool_impl.hpp>
#include <boost/thread/thread.hpp>
#include <iostream>

using namespace tsbe;

NUMAMask tsbe::NUMA_MASK_NULL = NUMAMask(~0);

static Theron::Framework::Parameters get_params(const ThreadPoolConfig &config)
{
    if (config.node_mask == NUMA_MASK_NULL and config.processor_mask == NUMA_MASK_NULL)
    {
        return Theron::Framework::Parameters(config.num_threads);
    }
    if (config.processor_mask == NUMA_MASK_NULL)
    {
        return Theron::Framework::Parameters(config.num_threads, config.node_mask);
    }
    return Theron::Framework::Parameters(config.num_threads, config.node_mask, config.processor_mask);
}

ThreadPoolImpl::ThreadPoolImpl(const ThreadPoolConfig &config):
    framework(get_params(config))
{
    //NOP
}

static size_t hardware_concurrency(void)
{
    const size_t n = boost::thread::hardware_concurrency();
    return std::max(size_t(3), n);
}

ThreadPoolConfig::ThreadPoolConfig(void)
{
    this->num_threads = hardware_concurrency();
    this->node_mask = NUMA_MASK_NULL;
    this->processor_mask = NUMA_MASK_NULL;
}

ThreadPool::ThreadPool(void)
{
    //NOP
}

ThreadPool::ThreadPool(boost::weak_ptr<ThreadPoolImpl> p):
    boost::shared_ptr<ThreadPoolImpl>(p.lock())
{
    //NOP
}

ThreadPool::ThreadPool(const ThreadPoolConfig &config)
{
    this->reset(new ThreadPoolImpl(config));
}

static boost::weak_ptr<ThreadPoolImpl> active_thread_pool;

ThreadPool ThreadPool::get_active(void)
{
    if (not active_thread_pool.lock())
    {
        ThreadPoolConfig config;
        std::cerr << "Creating default thread pool with " << config.num_threads << " num threads..." << std::endl;
        ThreadPool tp(config);
        tp.activate();
        return tp;
    }
    return active_thread_pool;
}

void ThreadPool::activate(void)
{
    active_thread_pool = *this;
}
