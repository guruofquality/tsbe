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

#include <boost/test/unit_test.hpp>
#include <boost/format.hpp>
#include <iostream>
#include <ctime>

#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>

//#include <sstream>

namespace ipc = boost::interprocess;

BOOST_AUTO_TEST_CASE(shared_mem_alloc_test0){
    const size_t len = ipc::mapped_region::get_page_size();//(1 << 16)*8;//1024*8;
    
    boost::uuids::uuid u1; // initialize uuid
    //std::stringstream ss;
    //ss << u1;
    std::string s1 = to_string(u1);
    std::cout << "s1 " << s1 << std::endl;
    
    std::srand(time(NULL));
    const std::string name = s1;//str(boost::format("shared_memory%d") % std::rand());

    std::cout << "make shmem\n" << std::endl;
    ipc::shared_memory_object shm_obj(
        ipc::create_only,                  //only create
        name.c_str(),              //name
        ipc::read_write                   //read-write mode
    );

    std::cout << "truncate\n" << std::endl;
    shm_obj.truncate(2*len);

    //find an address that can be mapped across 2x length
    char *addr = NULL;

    {
        std::cout << "map region 0\n" << std::endl;
        ipc::mapped_region region0(
            shm_obj,                      //Memory-mappable object
            ipc::read_write,               //Access mode
            0,                //Offset from the beginning of shm
            2*len        //Length of the region
        );
        std::cout << "region0.get_address() " << size_t(region0.get_address()) << std::endl;

        addr = (char *)region0.get_address();

    }

    std::cout << "map region 1\n" << std::endl;
    ipc::mapped_region region1(
        shm_obj,                      //Memory-mappable object
        ipc::read_write,               //Access mode
        0,                //Offset from the beginning of shm
        len,        //Length of the region
        addr
    );
    std::cout << "region1.get_address() " << size_t(region1.get_address()) << std::endl;

    std::cout << "map region 2\n" << std::endl;
    ipc::mapped_region region2(
        shm_obj,                      //Memory-mappable object
        ipc::read_write,               //Access mode
        0,                //Offset from the beginning of shm
        len,        //Length of the region
        addr + len
    );

    std::cout << "region2.get_address() " << size_t(region2.get_address()) << std::endl;
    std::cout << "diff " << (long(region2.get_address()) - long(region1.get_address())) << std::endl;

    std::memset(region1.get_address(), 0, region1.get_size());
    std::memset(region2.get_address(), 0, region2.get_size());
    ((char *)region1.get_address())[11] = char(rand());
    std::cout << "((char *)region1.get_address())[11] " << int(((char *)region1.get_address())[11]) << std::endl;
    std::cout << "((char *)region1.get_address())[len + 11] " << int(((char *)region1.get_address())[len + 11]) << std::endl;
    std::cout << "((char *)region2.get_address())[11] " << int(((char *)region2.get_address())[11]) << std::endl;

    std::cout << "free\n" << sizeof(long) << std::endl;
    ipc::shared_memory_object::remove(name.c_str());

}
