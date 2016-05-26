cmake_minimum_required(VERSION 2.8.4)

project(psk)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -std=c11")

set(SOURCE_LIB psk.c aes128.c aescmac.c)

include_directories(../lower)
include_directories(../eap)
include_directories(../auth_eap)

add_library(psk STATIC ${SOURCE_LIB})

add_library(auth STATIC IMPORTED)
set_property(TARGET auth PROPERTY IMPORTED_LOCATION ../libauth.a)
add_library(eap STATIC IMPORTED)
set_property(TARGET eap PROPERTY IMPORTED_LOCATION ../libeap.a)
add_library(lower STATIC IMPORTED)
set_property(TARGET lower PROPERTY IMPORTED_LOCATION ../liblower.a)

target_link_libraries(psk m)
target_link_libraries(psk auth)
target_link_libraries(psk eap)
target_link_libraries(psk lower)
