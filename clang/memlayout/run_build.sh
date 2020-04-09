#!/bin/bash

cd build && cmake .. -DCMAKE_BUILD_TYPE=Debug && make -j4

if [ 0 -eq $? ]; then
  ./memlayout -p='../example/build' .
fi
