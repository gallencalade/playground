#!/bin/bash

cd build && cmake .. -DCMAKE_BUILD_TYPE=Debug && make -j4

./memlayout -p='../example/build' .
