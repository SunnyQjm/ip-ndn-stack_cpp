cmake_minimum_required(VERSION 3.12)
project(ip_ndn_stack_cpp)

set(CMAKE_CXX_STANDARD 14)

include_directories(${PROJECT_SOURCE_DIR}/helper/)
add_executable(ip_ndn_stack_cpp main.cpp)