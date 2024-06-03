// address.h
// Created by Brian Cassriel on 5/7/24.
// The Address class holds the page and offset of a virtual address.

#ifndef VIRTUALMEMORYMANAGER_ADDRESS_H
#define VIRTUALMEMORYMANAGER_ADDRESS_H

#include <cstdlib>

class Address {
public:
    u_int8_t page;
    u_int8_t offset;

    /** Address constructor
     * Initializes the page and offset to 0.
     */
    Address() {
        this->page = 0;
        this->offset = 0;
    }
};

#endif //VIRTUALMEMORYMANAGER_ADDRESS_H
