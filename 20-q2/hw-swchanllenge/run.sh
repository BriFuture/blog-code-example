#!/bin/bash

cd build/
g++ -O3 ../main.cpp -o test -lpthread
if [ $? -ne 0 ]; then
       exit 1;
fi

echo "start: " `date`
# ./test
perf stat -e cycles,cache-misses ./test
echo "end: " `date`
