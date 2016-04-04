cmake_minimum_required(VERSION 2.8)

project(lower)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -std=c11")

set(SOURCE_LIB lower.c)

add_library(lower STATIC ${SOURCE_LIB}) 
