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

#ifndef INCLUDED_TSBE_WAX_HPP
#define INCLUDED_TSBE_WAX_HPP

#include <tsbe/config.hpp>
#include <typeinfo>
#include <new>

namespace tsbe
{

/*!
 * Wax is a lightweight, type-agnostic container, inspired by boost's any.
 * Wax offers performance over boost's any at the expense of flexibility.
 *
 * Wax uses uses a fixed-sized buffer and placement new for allocation.
 * This avoids bulky calls to malloc, making Wax "lightweight",
 * but limits what Wax can store due to the fixed-size.
 *
 * Wax should be used to store small items like:
 * integers, floats, and in particular, shared and intrusive pointers.
 * Larger objects cannot be supported due to the fixed-size buffer.
 */
class TSBE_API Wax
{
public:

    //! The fixed size of the storage buffer in bytes
    static const unsigned buff_size = 64;

    //! Create an empty Wax object
    Wax(void)
    {
        item = NULL;
    }

    //! Create a Wax object container from any type
    template <typename ValueType>
    Wax(const ValueType &value)
    {
        item = new ((void *)buff) Container<ValueType>(value);
    }

    //! Copy constructor for Wax object
    Wax(const Wax &obj)
    {
        item = (obj.empty())? NULL : obj.item->clone(buff);
    }

    //! Deconstructor
    ~Wax(void)
    {
        this->reset();
    }

    /*!
     * Reset this Wax object.
     * If we are holding an item, delete it.
     * This object will be empty after reset.
     */
    void reset(void)
    {
        if (item) item->reset();
        item = NULL;
    }

    /*!
     * Is this object empty?
     * Empty means that the object does not hold an item.
     */
    bool empty(void) const
    {
        return item == NULL;
    }

    //! Load this Wax object with an item from an arbitrary assignment
    template<typename ValueType>
    Wax &operator=(const ValueType &rhs)
    {
        this->reset();
        item = new ((void *)buff) Container<ValueType>(rhs);
        return *this;
    }

    //! Copy assignment of a Wax object
    Wax &operator=(const Wax &rhs)
    {
        this->reset();
        if (not rhs.empty()) item = rhs.item->clone(buff);
        return *this;
    }

    /*!
     * Get the type of the item held.
     * Only call if this object is not empty.
     */
    const std::type_info &type(void) const
    {
        return item->type();
    }

    /*!
     * Cast the item held by this object to an arbitrary type.
     * Only call if this object is not empty.
     */
    template <typename ValueType>
    ValueType cast(void) const
    {
        return static_cast<Container<ValueType> *>(item)->value;
    }

private:
    struct Item
    {
        virtual void reset(void) = 0;
        virtual Item *clone(void *) const = 0;
        virtual const std::type_info &type(void) const = 0;
    } *item;

    template <typename ValueType>
    struct Container : Item
    {
        Container(const ValueType &value):
            value(value)
        {
            //NOP
        }

        void reset(void)
        {
            value = ValueType();
        }

        const std::type_info &type(void) const
        {
            return typeid(ValueType);
        }

        Item *clone(void *buff) const
        {
            return new (buff) Container<ValueType>(value);
        }

        ValueType value;
    };

    char buff[buff_size];
};

} //namespace tsbe

#endif /*INCLUDED_TSBE_WAX_HPP*/
