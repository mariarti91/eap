cmake_minimum_required(VERSION 2.8)

project(eap_server)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -std=c11")

set(SOURCE_ELF main.c)

include_directories(../libs/psk)
include_directories(../libs/auth_eap)
include_directories(../libs/eap)

add_executable(eap_server.elf ${SOURCE_ELF})

add_library(psk STATIC IMPORTED)
set_property(TARGET psk PROPERTY IMPORTED_LOCATION ../../libs/libpsk.a)

add_library(auth STATIC IMPORTED)
set_property(TARGET auth PROPERTY IMPORTED_LOCATION ../../libs/libauth.a)

add_library(eap STATIC IMPORTED)
set_property(TARGET eap PROPERTY IMPORTED_LOCATION ../../libs/libeap.a)

add_library(lower STATIC IMPORTED)
set_property(TARGET lower PROPERTY IMPORTED_LOCATION ../../libs/liblower.a)

target_link_libraries(eap_server.elf psk auth eap lower)
