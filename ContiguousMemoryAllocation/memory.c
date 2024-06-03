// memory.c
// Created by Brian Cassriel on 4/15/24.
// This file contains the memory allocation functions for the memory data structure.

#include "memory.h"
#include "list.h"

/** memoryIsEmpty
 * Checks if the memory is empty
 * @param memory - the memory structure
 * @return true if the memory is empty, false otherwise
 */
bool memoryIsEmpty(const Memory *memory) { return getSize(memory->memBlocks) == 0; }

/** firstBlockIsFree
 * Checks if the first block in the memory is free
 * @param memory - the memory structure
 * @return true if the first block is free, false otherwise
 */
bool firstBlockIsFree(const Memory *memory) { return memory->memBlocks->headNode->next->block->start != memory->start; }

/** atEndOfList
 * Checks if the request command has reached the end of the list
 * @param nextBlock - the next block
 * @return true if the next block is NULL, false otherwise
 */
bool atEndOfList(const Block *nextBlock) { return nextBlock == NULL; }

/** isSpaceBetweenTwoBlocks
 * Checks if there is space between the current block and the next block
 * @param currBlock - the current block
 * @param nextBlock - the next block
 * @return true if there is space between the two blocks, false otherwise
 */
bool isSpaceBetweenTwoBlocks(const Block *currBlock, const Block *nextBlock) { return currBlock->end + 1 != nextBlock->start; }

/** fitsInLocation
 * Checks if the process fits in the location
 * @param process - the process
 * @param start - the start location
 * @param end - the end location
 * @return true if the process fits in the location, false otherwise
 */
bool fitsInLocation(const Block *process, const int start, const int end) { return process->size <= end - start + 1; }

/** tryAppend
 * Tries to append a block to the memory with some error checking
 * @param memory - the memory structure
 * @param process - the Block to append
 * @param start - the start location
 * @param end - the end location
 */
void tryAppend(Memory *memory, Block *process, int start, int end) {
    if (fitsInLocation(process, start, end)) {
        process->start = start;
        process->end = process->start + process->size - 1;
        append(memory->memBlocks, process);
    } else {
        printf("Process has not been allocated. No space available.\n");
    }
}

/** tryPrepend
 * Tries to prepend a block to the memory with some error checking
 * @param memory - the memory structure
 * @param process - the Block to prepend
 * @param end - the end location
 */
void tryPrepend(Memory *memory, Block *process, int start, int end) {
    if (fitsInLocation(process, start, end)) {
        process->start = memory->start;
        process->end = process->start + process->size - 1;
        prepend(memory->memBlocks, process);
    } else {
        printf("Process has not been allocated. No space available.\n");
    }
}

/** findFirstFit
 * Attempts to fit the process in the memory using the first fit algorithm
 * @param memory - the memory structure
 * @param process - the process to allocate
 */
void findFirstFit(Memory *memory, Block *process) {
    Block* currBlock;
    Block* nextBlock;
    int i;

    i = 0;
    do {
        currBlock = getBlock(memory->memBlocks, i);
        nextBlock = getBlock(memory->memBlocks, i + 1);

        if (memoryIsEmpty(memory)) {
            tryAppend(memory, process, memory->start, memory->end);
            return;
        } else if (firstBlockIsFree(memory)) {
            tryPrepend(memory, process, memory->start, currBlock->start - 1);
            return;
        } else if (atEndOfList(nextBlock)) {
            tryAppend(memory, process, currBlock->end + 1, memory->end);
            return;
        } else if (isSpaceBetweenTwoBlocks(currBlock, nextBlock) && fitsInLocation(process, currBlock->end, nextBlock->start)) {
            process->start = currBlock->end + 1;
            process->end = process->start + process->size - 1;
            insertAfter(memory->memBlocks, process, i);
            return;
        }

        i++;
    } while (nextBlock != NULL);

    printf("Process has not been allocated. No space available.\n");
}

/** findBestFit
 * Attempts to fit the process in the memory using the best fit algorithm
 * @param memory - the memory structure
 * @param process - the process to allocate
 */
void findBestFit(Memory *memory, Block *process) {
    int bestFreeIdx = -2;
    int bestFreeSize = memory->size;
    Block* currBlock;
    Block* nextBlock;
    int i;

    i = 0;
    do {
        currBlock = getBlock(memory->memBlocks, i);
        nextBlock = getBlock(memory->memBlocks, i + 1);

        if (memoryIsEmpty(memory)) {
            tryAppend(memory, process, memory->start, memory->end);
            return;
        }
        if (firstBlockIsFree(memory) && fitsInLocation(process, memory->start, currBlock->start)) {
            if (currBlock->start - memory->start + 1 < bestFreeSize) {
                bestFreeSize = currBlock->start - memory->start - 1;
                bestFreeIdx = -1;
            }
        }
        if (atEndOfList(nextBlock) && fitsInLocation(process, currBlock->end, memory->end)) {
            if (memory->end - currBlock->end + 1 < bestFreeSize) {
                bestFreeSize = memory->end - currBlock->end - 1;
                bestFreeIdx = i;
            }
        }
        if (!atEndOfList(nextBlock) && isSpaceBetweenTwoBlocks(currBlock, nextBlock) && fitsInLocation(process, currBlock->end, nextBlock->start)) {
            if (nextBlock->start - currBlock->end + 1 < bestFreeSize) {
                bestFreeSize = nextBlock->start - currBlock->end - 1;
                bestFreeIdx = i;
            }
        }

        i++;
    } while (nextBlock != NULL);

    if (bestFreeIdx == -1)
        tryPrepend(memory, process, memory->start, memory->memBlocks->headNode->next->block->start - 1);
    else if (bestFreeIdx == -2)
        printf("Process has not been allocated. No space available.\n");
    else {
        process->start = getBlock(memory->memBlocks, bestFreeIdx)->end + 1;
        process->end = process->start + process->size - 1;
        insertAfter(memory->memBlocks, process, bestFreeIdx);
    }
}

/** findWorstFit
 * Attempts to fit the process in the memory using the worst fit algorithm
 * @param memory - the memory structure
 * @param process - the process to allocate
 */
void findWorstFit(Memory *memory, Block *process) {
    int worstFreeIdx = -2;
    int worstFreeSize = process->size;
    Block* currBlock;
    Block* nextBlock;
    int i;

    i = 0;
    do {
        currBlock = getBlock(memory->memBlocks, i);
        nextBlock = getBlock(memory->memBlocks, i + 1);

        if (memoryIsEmpty(memory)) {
            tryAppend(memory, process, memory->start, memory->end);
            return;
        }
        if (firstBlockIsFree(memory) && fitsInLocation(process, memory->start, currBlock->start)) {
            if (currBlock->start - memory->start + 1 > worstFreeSize) {
                worstFreeSize = currBlock->start - memory->start - 1;
                worstFreeIdx = -1;
            }
        }
        if (atEndOfList(nextBlock) && fitsInLocation(process, currBlock->end, memory->end)) {
            if (memory->end - currBlock->end + 1 > worstFreeSize) {
                worstFreeSize = memory->end - currBlock->end - 1;
                worstFreeIdx = i;
            }
        }
        if (!atEndOfList(nextBlock) && isSpaceBetweenTwoBlocks(currBlock, nextBlock) && fitsInLocation(process, currBlock->end, nextBlock->start)) {
            if (nextBlock->start - currBlock->end + 1 > worstFreeSize) {
                worstFreeSize = nextBlock->start - currBlock->end - 1;
                worstFreeIdx = i;
            }
        }

        i++;
    } while (nextBlock != NULL);

    if (worstFreeIdx == -1)
        tryPrepend(memory, process, memory->start, memory->memBlocks->headNode->next->block->start - 1);
    else if (worstFreeIdx == -2)
        printf("Process has not been allocated. No space available.\n");
    else {
        process->start = getBlock(memory->memBlocks, worstFreeIdx)->end + 1;
        process->end = process->start + process->size - 1;
        insertAfter(memory->memBlocks, process, worstFreeIdx);
    }
}

/** request
 * Processes the request command
 * @param memory - the memory structure
 * @param command - the command
 */
void request(Memory *memory, char** command) {
    if (command[1] == NULL || command[2] == NULL || command[3] == NULL) {
        printf("Invalid request. Usage: RQ <Process Name> <# Bytes> <Fit Method>\n");
        return;
    }

    Block *process = malloc(sizeof(Block));
    process->name = malloc(100 * sizeof(char));
    strcpy(process->name, command[1]);
    process->size = atoi(command[2]);
    char type = *command[3];

     if (type == 'F') {
         findFirstFit(memory, process);
     } else if (type == 'B') {
         findBestFit(memory, process);
     } else if (type == 'W') {
         findWorstFit(memory, process);
     } else {
         printf("Invalid type.\n");
         free(process);
         return;
     }
}

/** release
 * Processes the release command
 * @param memory - the memory structure
 * @param command - the command
 */
void release(Memory *memory, char** command) {
    if (command[1] == NULL) {
        printf("Invalid release. Usage: RL <Process Name>\n");
        return;
    }

    char* processName = command[1];

    for(int i = 0; i < getSize(memory->memBlocks); i++) {
        Block *currBlock = getBlock(memory->memBlocks, i);
        if (strcmp(currBlock->name, processName) == 0) {
            delete(memory->memBlocks, i);
            printf("Process %s has been removed. %d-%d now freed.\n", processName, currBlock->start, currBlock->end);
            return;
        }
    }
    printf("Process not found.\n");
}

/** compact
 * Compacts the memory so all blocks are contiguous with all free space at the end
 * @param memory - the memory structure
 */
void compact(Memory *memory) {
    int start = memory->start;

    for (int i = 0; i < getSize(memory->memBlocks); i++) {
        Block *currBlock = getBlock(memory->memBlocks, i);
        currBlock->start = start;
        currBlock->end = currBlock->start + currBlock->size - 1;
        start = currBlock->end + 1;
    }
    printf("Memory has been compacted.\n");
}

/** stats
 * Prints the used and unused memory blocks as well as the total memory size
 * @param memory - the memory structure
 */
void stats(Memory *memory) {
    printf("Allocated Memory: %d (%d-%d)\n", memory->size, memory->start, memory->end);
    printf("Processes:\n");
    int memSize = getSize(memory->memBlocks);

    if (memSize == 0) {
        printf("    Unused: %d (%d-%d)\n", memory->size, memory->start, memory->end);
    } else {
        int prevEnd = memory->start - 1;
        for (int i = 0; i < memSize; i++) {
            Block *currBlock = getBlock(memory->memBlocks, i);
            if (currBlock->start - prevEnd > 1) {
                printf("    Unused: %d (%d-%d)\n", currBlock->start - prevEnd - 1, prevEnd + 1, currBlock->start - 1);
            }
            printf("    %s: %d (%d-%d)\n", currBlock->name, currBlock->size, currBlock->start, currBlock->end);
            prevEnd = currBlock->end;
        }
        if (memory->end - prevEnd > 0) {
            printf("    Unused: %d (%d-%d)\n", memory->end - prevEnd, prevEnd + 1, memory->end);
        }
    }
}
