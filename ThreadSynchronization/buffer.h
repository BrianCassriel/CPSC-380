// buffer header file
// Created by Brian Cassriel on 4/5/24.
// The buffer is a circular buffer that is used to pass BufferItems between two threads
// The buffer is protected by a mutex and two semaphores, which are used to signal when the buffer is full or empty

#ifndef THREADSYNCHRONIZATION_BUFFER_H
#define THREADSYNCHRONIZATION_BUFFER_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <pthread.h>
#include <fcntl.h>
#include <semaphore.h>

#define BUFFER_SIZE 10 /// Size of the buffer

/** BufferItem
 * Structure that holds the data and checksum for a buffer item
 */
typedef struct buffer_item {
    uint8_t data[30];
    uint16_t checksum;
} BufferItem;

/** Buffer
 * Structure that holds the buffer and keeps track of the head and tail of the buffer
 * Also contains a mutex and two semaphores to protect itself
 */
typedef struct buffer{
    BufferItem items[BUFFER_SIZE];
    int head;
    int tail;
    pthread_mutex_t mutex;
    sem_t* empty;
    sem_t* full;
} Buffer;

void initBuffer(Buffer *buffer);
void destructBuffer(Buffer *buffer);
int enqueue(Buffer *buffer, BufferItem item);
int dequeue(Buffer *buffer, BufferItem *item);
int isEmpty(Buffer *buffer);
int isFull(Buffer *buffer);

#endif //THREADSYNCHRONIZATION_BUFFER_H
