// PhysicalMemory.h
// Created by Brian Cassriel on 5/7/24.
// The PhysicalMemory class is a simple vector that represents the physical memory of the system.

#ifndef VIRTUALMEMORYMANAGER_PHYSICALMEMORY_H
#define VIRTUALMEMORYMANAGER_PHYSICALMEMORY_H

#include <cstdlib>
#include <vector>

#define NUM_FRAMES 256
#define FRAME_SIZE 256

class PhysicalMemory {
public:
    PhysicalMemory(int frameCount);
    ~PhysicalMemory();
    u_int8_t addOrFindFrame(u_int8_t page, char *data);
    int getByte(u_int8_t frame, u_int8_t offset);

private:
    std::vector<char *> memory;
};


#endif //VIRTUALMEMORYMANAGER_PHYSICALMEMORY_H
