cmake_minimum_required(VERSION 2.8)

project(eap)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -std=c11")

set(SOURCE_LIB eap.c)

add_library(eap STATIC ${SOURCE_LIB}) 
