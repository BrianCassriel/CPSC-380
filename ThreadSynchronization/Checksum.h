// Checksum header file
// Created by Brian Cassriel on 4/7/24.
// The checksum is used to verify the integrity of the data in the buffer

#ifndef THREADSYNCHRONIZATION_CHECKSUM_H
#define THREADSYNCHRONIZATION_CHECKSUM_H

#include <stdint.h>                 // Needed for int sizes
#include <stdlib.h>		            // Needed for rand()

uint16_t checksum(char *addr, uint32_t count);

#endif //THREADSYNCHRONIZATION_CHECKSUM_H
