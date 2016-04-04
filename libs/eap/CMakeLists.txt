cmake_minimum_required(VERSION 2.8)

project(eap)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -std=c11")

set(SOURCE_LIB eap.c)

include_directories(../lower)

add_library(eap STATIC ${SOURCE_LIB})

add_library(lower STATIC IMPORTED)
set_property(TARGET lower PROPERTY IMPORTED_LOCATION ../liblower.a)

target_link_libraries(eap lower)
