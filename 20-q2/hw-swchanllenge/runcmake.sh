#!/bin/sh
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j3
echo "start: " `date`
./Main
echo "end: `date`"