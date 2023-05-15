cmake_minimum_required(VERSION 3.24)
project(W5_CPP)

set(CMAKE_CXX_STANDARD 98)

add_executable(W5_CPP main.cpp fileProc.cpp fileProc.h logger.cpp logger.h)
