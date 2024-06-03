#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <address in dec>\n", argv[0]);
        return 1;
    }

    u_int32_t dec = atoi(argv[1]);
    u_int32_t pageNumOneKB = 0;
    u_int32_t offsetOneKB = 0;
    u_int32_t pageNumTwoKB = 0;
    u_int32_t offsetTwoKB = 0;

    pageNumOneKB = dec >> 10; // bitshift 10 to the right to not use the last 10 bits
    offsetOneKB = dec & 0x000003FF; // binary AND to get the last 10 bits

    pageNumTwoKB = dec >> 11; // bitshift 11 to the right to not use the last 11 bits
    offsetTwoKB = dec & 0x000007FF; // binary AND to get the last 11 bits

    printf("Address: 0x%x\n", dec);

    printf("One KB page size\n");
    printf("page number: %u\n", pageNumOneKB);
    printf("offset: %u\n", offsetOneKB);
    printf("\n");
    printf("Two KB page size\n");
    printf("page number: %u\n", pageNumTwoKB);
    printf("offset: %u\n", offsetTwoKB);
    return 0;
}
