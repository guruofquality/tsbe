########################################################################
## TSBE
########################################################################

------------------------------------------------------------------------
-- Prerequisites
------------------------------------------------------------------------
* C++ compiler (gcc and MSVC tested)
* CMake
* Boost - boost headers, and the thread library
* libnuma (linux only)

------------------------------------------------------------------------
-- Get the source code
------------------------------------------------------------------------
git clone https://github.com/guruofquality/tsbe.git
cd tsbe
git submodule init
git submodule update

------------------------------------------------------------------------
-- Installation instructions
------------------------------------------------------------------------
mkdir <build dir>
cd <build dir>
cmake <source dir>
make
sudo make install
