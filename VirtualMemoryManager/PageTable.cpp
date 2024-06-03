// PageTable.cpp
// Created by Brian Cassriel on 5/7/24.
// The PageTable class is a simple hash map that maps page numbers to frame numbers.

#include "PageTable.h"

/** PageTable constructor
 * Initializes the pageFaults counter.
 */
PageTable::PageTable() {
    pageFaults = 0;
}

/** PageTable destructor
 */
PageTable::~PageTable() {
}

/** addEntry
 * Adds a page/frame entry to the page table.
 * @param page The page number.
 * @param frame The frame number.
 */
void PageTable::addEntry(u_int8_t page, u_int8_t frame) {
    pageTable[page] = frame;
}

/** getFrame
 * Gets the frame number for a given page number.
 * @param page The page number.
 * @return The frame number.
 */
int PageTable::getFrame(u_int8_t page) {
    return pageTable[page];
}

/** containsPage
 * Checks if the page table contains a given page number.
 * @param page The page number.
 * @return True if the page is in the table, false otherwise.
 */
bool PageTable::containsPage(u_int8_t page) {
    if (pageTable.find(page) != pageTable.end())
        return true;
    else {
        pageFaults++;
        return false;
    }
}

/** getFaultRate
 * Gets the page fault rate.
 * @return The page fault rate.
 */
int PageTable::getFaultRate() {
    return pageFaults;
}
