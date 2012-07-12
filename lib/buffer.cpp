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

#include <tsbe/buffer.hpp>
#include <boost/detail/atomic_count.hpp>

using namespace tsbe;

struct tsbe::BufferImpl
{
    BufferImpl(const BufferConfig &config):
        count(0),
        config(config)
    {
        if (config.memory == NULL)
        {
            this->default_allocator();
        }
        this->reset();
    }

    void default_allocator(void)
    {
        //TODO call numanuma
    }

    void reset(void)
    {
        this->memory = this->config.memory;
        this->length = this->config.length;
    }

    boost::detail::atomic_count count;
    size_t __pad; //avoid a probably non-issue w/ count
    BufferConfig config;
    void *memory;
    size_t length;
};

void tsbe::intrusive_ptr_add_ref(BufferImpl *impl)
{
    ++impl->count;
}

void tsbe::intrusive_ptr_release(BufferImpl *impl)
{
    if (--impl->count == 0)
    {
        //reset the memory and length
        impl->reset();

        //call the deleter if possible
        boost::shared_ptr<BufferDeleter> token_deleter = impl->config.token.lock();
        if (token_deleter)
        {
            Buffer buff;
            buff.reset(impl);
            (*token_deleter)(buff);
        }
        else if (impl->config.deleter)
        {
            Buffer buff;
            buff.reset(impl);
            impl->config.deleter(buff);
        }
        //else just let it wither away here...
    }
}

BufferConfig::BufferConfig(void)
{
    memory = NULL;
    length = 0;
    affinity = TSBE_AFFINITY_NONE;
}

Buffer::Buffer(void)
{
    //NOP
}

Buffer::Buffer(const BufferConfig &config)
{
    this->reset(new BufferImpl(config));
}

void *Buffer::get_actual_memory(void) const
{
    return (*this)->config.memory;
}

size_t Buffer::get_actual_length(void) const
{
    return (*this)->config.length;
}

void *&Buffer::get_memory(void)
{
    return (*this)->memory;
}

size_t &Buffer::get_length(void)
{
    return (*this)->length;
}

size_t Buffer::get_affinity(void) const
{
    return (*this)->config.affinity;
}
