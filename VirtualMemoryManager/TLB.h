// TLB.h
// Created by Brian Cassriel on 5/5/24.
//

#ifndef VIRTUALMEMORYMANAGER_TLB_H
#define VIRTUALMEMORYMANAGER_TLB_H

#define MAX_TLB_SIZE 16

#include <map>
#include <cstdlib>

class TLB {
public:
    TLB();
    ~TLB();
    void addEntry(u_int8_t page, u_int8_t frame);
    int getFrame(u_int8_t page);
    bool containsPage(u_int8_t page);
    int getHitRate();

private:
    std::map<u_int8_t, u_int8_t> tlb;
    std::map<u_int8_t, int> lru;
    int tlbHits;

    void removeLRU();
};


#endif //VIRTUALMEMORYMANAGER_TLB_H
