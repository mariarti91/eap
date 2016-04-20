cmake_minimum_required(VERSION 2.8)

project(auth)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -std=c11")

set(SOURCE_LIB auth.c)

include_directories(../lower)
include_directories(../eap)

add_library(auth STATIC ${SOURCE_LIB})

add_library(eap STATIC IMPORTED)
set_property(TARGET eap PROPERTY IMPORTED_LOCATION ../libeap.a)
add_library(lower STATIC IMPORTED)
set_property(TARGET lower PROPERTY IMPORTED_LOCATION ../liblower.a)

target_link_libraries(auth eap)
target_link_libraries(auth lower)
