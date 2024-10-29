cmake_minimum_required(VERSION 3.10)
project(kry)
add_executable(kry main.cpp kry.cpp)
target_include_directories(kry PUBLIC ./)
