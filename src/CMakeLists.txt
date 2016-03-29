cmake_minimum_required(VERSION 2.8)

project(eap_peer)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -std=c11")

set(SOURCE_ELF main.c)

add_executable(eap_peer.elf ${SOURCE_ELF}) 
