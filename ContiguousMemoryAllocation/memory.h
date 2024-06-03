// memory.h
// Created by Brian Cassriel on 4/15/24.
// This file contains the memory data structure and the function prototypes for memory.c.

#ifndef CONTIGUOUSMEMORYALLOCATION_MEMORY_H
#define CONTIGUOUSMEMORYALLOCATION_MEMORY_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct list List;

typedef struct block {
    char* name;
    int start;
    int end;
    int size;
} Block;

typedef struct memory {
    int start;
    int end;
    int size;
    List* memBlocks;
} Memory;

void request(Memory *memory, char** command);
void release(Memory *memory, char** command);
void compact(Memory *memory);
void stats(Memory *memory);

#endif //CONTIGUOUSMEMORYALLOCATION_MEMORY_H
