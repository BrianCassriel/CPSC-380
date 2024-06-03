// TLB.cpp
// Created by Brian Cassriel on 5/5/24.
// The TLB class is a simple associative array that represents the Translation Lookaside Buffer (TLB).

#include "TLB.h"

/** TLB constructor
 * Initializes the TLB.
 */
TLB::TLB() {
    tlbHits = 0;
}

/** TLB destructor
 */
TLB::~TLB() {
}

/** addEntry
 * Adds an entry to the TLB.
 * @param page The page number.
 * @param frame The frame number.
 */
void TLB::addEntry(u_int8_t page, u_int8_t frame) {
    if (tlb.find(page) != tlb.end()) {
        tlb[page] = frame;
        for (std::pair<const unsigned char, int> & i : lru) {
            i.second++;
        }
    } else {
        if (tlb.size() == MAX_TLB_SIZE) {
            removeLRU();
            tlb[page] = frame;
            lru[page] = 0;
        } else {
            tlb[page] = frame;
            for (std::pair<const unsigned char, int>& i : lru) {
                i.second++;
            }
            lru[page] = 0;
        }
    }
}

/** removeLRU
 * Removes the least recently used entry from the TLB.
 */
void TLB::removeLRU() {
    int longestSinceUse = 0;
    u_int8_t oldestPage;

    for (std::pair<const unsigned char, int> currPage : lru) {
        if (currPage.second > longestSinceUse) {
            longestSinceUse = currPage.second;
            oldestPage = currPage.first;
        }
    }
    lru.erase(oldestPage);
    tlb.erase(oldestPage);
}

/** getFrame
 * Gets the frame number from the TLB.
 * @param page The page number.
 * @return The frame number.
 */
int TLB::getFrame(u_int8_t page) {
    return tlb[page];
}

/** containsPage
 * Checks if the TLB contains the page.
 * @param page The page number.
 * @return True if the page is in the TLB.
 */
bool TLB::containsPage(u_int8_t page) {
    if (tlb.find(page) != tlb.end()) {
        tlbHits++;
        return true;
    } else {
        return false;
    }
}

/** getHitRate
 * Gets the hit rate of the TLB.
 * @return The hit rate.
 */
int TLB::getHitRate() {
    return tlbHits;
}
