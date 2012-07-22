//
// Copyright 2011-2012 Josh Blum
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

#ifndef INCLUDED_TSBE_CONFIG_HPP
#define INCLUDED_TSBE_CONFIG_HPP

// http://gcc.gnu.org/wiki/Visibility
// Generic helper definitions for shared library support
#if defined _WIN32 || defined __CYGWIN__
  #define TSBE_HELPER_DLL_IMPORT __declspec(dllimport)
  #define TSBE_HELPER_DLL_EXPORT __declspec(dllexport)
  #define TSBE_HELPER_DLL_LOCAL
#else
  #if __GNUC__ >= 4
    #define TSBE_HELPER_DLL_IMPORT __attribute__ ((visibility ("default")))
    #define TSBE_HELPER_DLL_EXPORT __attribute__ ((visibility ("default")))
    #define TSBE_HELPER_DLL_LOCAL  __attribute__ ((visibility ("hidden")))
  #else
    #define TSBE_HELPER_DLL_IMPORT
    #define TSBE_HELPER_DLL_EXPORT
    #define TSBE_HELPER_DLL_LOCAL
  #endif
#endif

// Now we use the generic helper definitions above to define TSBE_API and TSBE_LOCAL.
// TSBE_API is used for the public API symbols. It either DLL imports or DLL exports (or does nothing for static build)
// TSBE_LOCAL is used for non-api symbols.

#ifdef TSBE_DLL // defined if TSBE is compiled as a DLL
  #ifdef TSBE_DLL_EXPORTS // defined if we are building the TSBE DLL (instead of using it)
    #define TSBE_API TSBE_HELPER_DLL_EXPORT
  #else
    #define TSBE_API TSBE_HELPER_DLL_IMPORT
  #endif // TSBE_DLL_EXPORTS
  #define TSBE_LOCAL TSBE_HELPER_DLL_LOCAL
#else // TSBE_DLL is not defined: this means TSBE is a static lib.
  #define TSBE_API
  #define TSBE_LOCAL
#endif // TSBE_DLL

#include <boost/config.hpp>

#ifdef BOOST_MSVC
#  pragma warning(disable: 4251) // class 'A<T>' needs to have dll-interface to be used by clients of class 'B'
#  include <ciso646>
#endif //BOOST_MSVC

// include smart pointer stuff for typedefs below
#include <boost/shared_ptr.hpp>
#include <boost/intrusive_ptr.hpp>
#include <boost/weak_ptr.hpp>

// include bitset here for the typedef below
#include <boost/dynamic_bitset.hpp>

//forward declares and stuff for library implementation
namespace tsbe
{

struct Buffer;
struct BufferImpl;

extern TSBE_API void intrusive_ptr_add_ref(BufferImpl*);
extern TSBE_API void intrusive_ptr_release(BufferImpl*);

struct Block;
struct BlockImpl;

struct ExecutorImpl;

struct TaskInterfaceImpl;
struct TaskInterface;

struct ElementImpl;
typedef boost::shared_ptr<ElementImpl> Element;

typedef boost::dynamic_bitset<> BitSet;

struct TSBE_API Affinity
{
    Affinity(void)
    {
        value = -1;
    }

    operator long long(void) const
    {
        return value;
    }

    long long value;
};

} //namespace tsbe

#define TSBE_MAX_ALIGNMENT 32

#include <iostream>

#define TSBE_HERE() {std::cout << __FILE__ << ":" << __LINE__ << std::endl;}
#define TSBE_VAR(var) {std::cout << #var << " = " << (var) << std::endl;}

#endif /*INCLUDED_TSBE_CONFIG_HPP*/
