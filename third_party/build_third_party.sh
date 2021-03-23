#!/bin/bash -e

# assume user is in third_party directory

cd googletest-release-1.10.0 
rm -rf build
mkdir build
cd build
# cmake -DBUILD_SHARED_LIBS:BOOL=ON ..
cmake ..
sudo make install

# copy the library files to make them available on the PATH
# sudo cp lib/*.a /usr/lib
sudo cp lib/*.a /usr/local/lib

# take user back to third_parth dir
cd ../../