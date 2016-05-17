cmake_minimum_required(VERSION 2.8.4)

project(cmac)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -std=c11")

set(SOURCE_ELF main.c aes128.c)

add_executable(cmac.elf ${SOURCE_ELF})
