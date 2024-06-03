cmake_minimum_required(VERSION 3.27)
project(ThreadSynchronization C)

set(CMAKE_C_STANDARD 11)

add_executable(prodcon prodcon.c
        Checksum.c
        buffer.h
        buffer.c
        Checksum.h)
