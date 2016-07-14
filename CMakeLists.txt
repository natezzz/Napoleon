cmake_minimum_required(VERSION 3.4.1)
project(napoleon)

include_directories(include)
file(GLOB SOURCES "src/*.cpp")

add_executable(napoleon ${SOURCES})
