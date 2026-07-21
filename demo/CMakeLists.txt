cmake_minimum_required(VERSION 3.20)

set (CMAKE_CXX_STANDARD 17)

project(LSM6DSOX_test LANGUAGES CXX)

include_directories(..)

add_executable(sample_printer sample_printer.cpp)

target_link_libraries(sample_printer LSM6DSOX_Rpi i2c)

add_compile_options(-Wall -pedantic)

set(CMAKE_BUILD_TYPE Debug)
