cmake_minimum_required(VERSION 3.27)
project(VirtualMemoryManager)

set(CMAKE_CXX_STANDARD 17)

add_executable(vmmgr vmmgr.cpp
        TLB.cpp
        TLB.h
        FileProcessor.cpp
        FileProcessor.h
        address.h
        address.h
        PageTable.cpp
        PageTable.h
        PageTable.h
        PhysicalMemory.cpp
        PhysicalMemory.h)
