cmake_minimum_required(VERSION 3.26)
project(graham_scan)

set(CMAKE_CXX_STANDARD 17)

add_executable(graham_scan main.cpp
        list.h
        array.h)
