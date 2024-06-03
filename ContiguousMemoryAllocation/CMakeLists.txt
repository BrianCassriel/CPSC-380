cmake_minimum_required(VERSION 3.27)
project(ContiguousMemoryAllocation C)

set(CMAKE_C_STANDARD 11)

add_executable(allocator allocator.c
        memory.h
        memory.c
        list.h
        list.c)
