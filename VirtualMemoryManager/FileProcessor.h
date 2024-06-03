// FileProcessor.h
// Created by Brian Cassriel on 5/5/24.
// The FileProcessor class handles reading the addresses from the file and reading the data from the backing store.

#ifndef VIRTUALMEMORYMANAGER_FILEPROCESSOR_H
#define VIRTUALMEMORYMANAGER_FILEPROCESSOR_H

#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <cstdlib>
#include "address.h"

#define VADDR_SIZE 65536
#define PAGE_SIZE 256
#define FRAME_SIZE 256
#define NUM_FRAMES 256

class FileProcessor {
public:
    FileProcessor(std::string addrFilePath);
    ~FileProcessor();
    int processFile(std::vector<Address*> &addrList);
    void getDataFromStore(u_int8_t page, char* data);

private:
    std::fstream addrFile;
    std::fstream backingStoreFile;
};


#endif //VIRTUALMEMORYMANAGER_FILEPROCESSOR_H
