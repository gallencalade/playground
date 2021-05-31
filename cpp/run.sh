#!/bin/bash

g++ test.cc -std=c++20 -lgtest -lgtest_main -pthread

./a.out
