cmake_minimum_required(VERSION 2.8)

project(eap_peer)

set(SOURCE_ELF main.c)

add_executable(eap_peer.elf ${SOURCE_ELF}) 
