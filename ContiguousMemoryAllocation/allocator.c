// allocator.c
// Created by Brian Cassriel on 4/15/24.
// This is file contains the main method for the memory allocation program.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "memory.h"
#include "list.h"

Memory* memory;

/** initMemory
 * Initializes the memory structure with the given size
 */
void initMemory(int size) {
    memory = malloc(sizeof(Memory));
    memory->size = size;
    memory->start = 0;
    memory->end = size - 1;

    memory->memBlocks = malloc(sizeof(List));
    initList(memory->memBlocks);
}

/** main
 * Main method for the memory allocation program.
 * This method reads in commands from the user and processes them.
 * At the end of the program, the memory is deallocated.
 * @param argc - number of arguments
 * @param argv - arguments
 */
int main(int argc, char** argv) {
    if (argc < 2) {
        printf("Usage: %s <number of bytes>\n", argv[0]);
        return 1;
    }

    int memSize = atoi(argv[1]);
    int shouldExit = 0;
    initMemory(memSize);

    while (shouldExit == 0) {
        printf("allocator> ");
        char** command = malloc(5 * sizeof(char));
        for (int i = 0; i < 5; i++) {
            command[i] = malloc(100 * sizeof(char));
        }
        scanf("%s", command[0]);

        if (strcmp(command[0], "RQ") == 0) {
            scanf("%s", command[1]);
            scanf("%s", command[2]);
            scanf("%s", command[3]);
            request(memory, command);
        } else if (strcmp(command[0], "RL") == 0) {
            scanf("%s", command[1]);
            release(memory, command);
        } else if (strcmp(command[0], "C") == 0) {
            compact(memory);
        } else if (strcmp(command[0], "STAT") == 0) {
            stats(memory);
        } else if (strcmp(command[0], "X") == 0) {
            shouldExit = 1;
        } else {
            printf("Invalid command\n");
        }

        for (int i = 0; i < 5; i++) {
            free(command[i]);
        }
        free(command);
    }

    deallocate(memory->memBlocks);
    free(memory->memBlocks);
    return 0;
}
