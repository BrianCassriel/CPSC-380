// PageTable.h
// Created by Brian Cassriel on 5/7/24.
// The PageTable class is a simple hash map that maps page numbers to frame numbers.

#ifndef VIRTUALMEMORYMANAGER_PAGETABLE_H
#define VIRTUALMEMORYMANAGER_PAGETABLE_H

#include <map>
#include <cstdlib>

class PageTable {
public:
    PageTable();
    ~PageTable();
    void addEntry(u_int8_t page, u_int8_t frame);
    int getFrame(u_int8_t page);
    bool containsPage(u_int8_t page);
    int getFaultRate();

private:
    std::map<u_int8_t, u_int8_t> pageTable;
    int pageFaults;
};


#endif //VIRTUALMEMORYMANAGER_PAGETABLE_H
