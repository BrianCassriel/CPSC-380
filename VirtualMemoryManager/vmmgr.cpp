// vmmgr.cpp
// Created by Brian Cassriel on 5/5/24.
// VMMGR simulates how addresses are converted from virtual memory to physical memory.

#include <iostream>

#include "FileProcessor.h"
#include "TLB.h"
#include "PageTable.h"
#include "PhysicalMemory.h"

/** main
 * Main function that reads in a file of addresses and processes them to simulate virtual memory to physical memory.
 * @param argc - number of arguments
 * @param argv - array of arguments
 * @return - 0 if successful, -1 if not
 */
int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cout << "Usage: ./vmmgr <address input file>" << std::endl;
        return -1;
    }
    std::string filename = argv[1];

    std::vector<Address *> addrList;
    FileProcessor *fileProcessor = new FileProcessor(filename);
    TLB *tlb = new TLB();
    PageTable *pageTable = new PageTable();
    PhysicalMemory *physicalMemory = new PhysicalMemory(NUM_FRAMES);
    u_int8_t currFrame;

    fileProcessor->processFile(addrList);
    for (Address* &currAddress : addrList) {
        std::cout << "Translating " << (int)currAddress->page << " " << (int)currAddress->offset << "... | ";

        if (tlb->containsPage(currAddress->page)) {
            currFrame = tlb->getFrame(currAddress->page);
            tlb->addEntry(currAddress->page, currFrame);
        } else if (pageTable->containsPage(currAddress->page)) {
            currFrame = pageTable->getFrame(currAddress->page);
            tlb->addEntry(currAddress->page, currFrame);
        } else {
            char *data = new char[FRAME_SIZE];
            fileProcessor->getDataFromStore(currAddress->page, data);

            currFrame = physicalMemory->addOrFindFrame(currAddress->page, data);
            pageTable->addEntry(currAddress->page, currFrame);
        }

        std::cout << "Frame number: " << (int)currFrame << " " << (int)currAddress->offset
                << " | Byte: " << physicalMemory->getByte(currFrame, currAddress->offset) << std::endl;
    }

    std::cout << "Page fault rate: " << pageTable->getFaultRate() << std::endl;
    std::cout << "TLB hit rate: " << tlb->getHitRate() << std::endl;

    delete tlb;
    delete pageTable;
    delete fileProcessor;
    return 0;
}
