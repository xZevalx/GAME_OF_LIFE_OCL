cmake_minimum_required(VERSION 3.12)
project(TheGameOfLife)
set(CMAKE_CXX_STANDARD 14)
include(FindOpenCL.cmake)

find_package(OPENCL REQUIRED)
include_directories(${OPENCL_INCLUDE_DIRS})

add_executable(TheGameOfLife main.cpp utils.h utils.cpp GameOfLife.cpp GameOfLife.h)
target_link_libraries(TheGameOfLife ${OPENCL_LIBRARIES})