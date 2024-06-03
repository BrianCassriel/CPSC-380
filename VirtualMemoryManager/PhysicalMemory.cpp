// PhysicalMemory.cpp
// Created by Brian Cassriel on 5/7/24.
// The PhysicalMemory class is a simple vector that represents the physical memory of the system.

#include "PhysicalMemory.h"

/** PhysicalMemory constructor
 * Initializes the memory vector with nullptrs.
 * @param frameCount The number of frames in the physical memory.
 */
PhysicalMemory::PhysicalMemory(int frameCount) {
    for (int i = 0; i < frameCount; i++) {
        memory.push_back(nullptr);
    }
}

/** PhysicalMemory destructor
 * Deletes the memory.
 */
PhysicalMemory::~PhysicalMemory() {
    for (char* & i : memory) {
        delete[] i;
    }
    memory.clear();
}

/** addOrFindFrame
 * Adds a frame to the memory or finds it if it already exists.
 * @param page The page number.
 * @param data The data to be added to the frame.
 * @return The frame number.
 */
u_int8_t PhysicalMemory::addOrFindFrame(u_int8_t page, char* data) {
    if (memory[page] != nullptr) {
        return page;
    } else {
        memory[page] = new char[FRAME_SIZE];
        for (int i = 0; i < FRAME_SIZE; i++) {
            memory[page][i] = data[i];
        }
        return page;
    }
}

/** getByte
 * Gets a byte from the memory.
 * @param frame The frame number.
 * @param offset The offset in the frame.
 * @return The byte.
 */
int PhysicalMemory::getByte(u_int8_t frame, u_int8_t offset) {
    return memory[frame][offset];
}
