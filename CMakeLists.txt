cmake_minimum_required(VERSION 3.5)

# Set C++11 standard for compile this code
set(CMAKE_CXX_STANDARD 11)

# Just project name
project(tk01 CXX)

# Project executable files
add_executable(tk01 stds.cpp)

install(TARGETS tk01 DESTINATION bin)

