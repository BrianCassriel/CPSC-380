// FileProcessor.cpp
// Created by Brian Cassriel on 5/5/24.
// The FileProcessor class handles reading the addresses from the file and reading the data from the backing store.

#include "FileProcessor.h"

/** FileProcessor constructor
 * @param addrFilePath The path to the file containing the addresses.
 */
FileProcessor::FileProcessor(const std::string addrFilePath) {
    this->addrFile.open(addrFilePath);
    this->backingStoreFile.open("BACKING_STORE.bin");
    if (!this->addrFile.is_open()) {
        std::cerr << "Error opening file: " << addrFilePath << std::endl;
        exit(1);
    }
    if (!this->backingStoreFile.is_open()) {
        std::cerr << "Error opening file: BACKING_STORE.bin. Please place it in the same directory as this program." << std::endl;
        exit(1);
    }
}

/** FileProcessor destructor
 * Closes the files.
 */
FileProcessor::~FileProcessor() {
    this->addrFile.close();
    this->backingStoreFile.close();
}

/** processFile
 * @param addrList The list of addresses to be filled.
 * @return 0 if successful, -1 if the file is not open.
 */
int FileProcessor::processFile(std::vector<Address*> &addrList) {
    if (!this->addrFile.is_open()) {
        return -1;
    }
    std::string line;
    while (getline(addrFile, line)) {
        if (VADDR_SIZE < std::stoi(line)) {
            std::cerr << "Address out of bounds: " << line << std::endl;
            exit(1);
        }
        Address *address = new Address();

        address->page = (std::stoi(line) >> 8) & 0xFF;
        address->offset = std::stoi(line) & 0xFF;
        addrList.push_back(address);
    }
    return 0;
}

/** getDataFromStore
 * @param page The page number to read from the backing store.
 * @param data The data to be filled.
 */
void FileProcessor::getDataFromStore(u_int8_t page, char* data) {
    this->backingStoreFile.seekg(page * PAGE_SIZE);
    this->backingStoreFile.read(data, FRAME_SIZE);
}
