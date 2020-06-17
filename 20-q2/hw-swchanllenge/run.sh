#!/bin/bash

cd build/
g++ -O3 ../main.cpp -o test -lpthread
echo "start: " `date`
./test
echo "end: " `date`
