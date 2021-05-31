#!/bin/bash

g++ test.cc -g -std=c++20 -lgtest -lgtest_main -pthread

./a.out
