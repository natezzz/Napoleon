cmake_minimum_required(VERSION 3.4.1)
project(napoleon)

include_directories(include)
file(GLOB SOURCES "src/*.cpp")

set (CMAKE_CXX_STANDARD 11)
add_executable(napoleon ${SOURCES})
