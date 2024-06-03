// buffer implementation file
// Created by Brian Cassriel on 4/7/24.
// The buffer is a circular buffer that is used to pass BufferItems between two threads
// The buffer is protected by a mutex and two semaphores, which are used to signal when the buffer is full or empty

#include "buffer.h"

/** initBuffer
 * Initializes the buffer with the mutex and semaphores
 * @param buffer - the buffer to initialize
 */
void initBuffer(Buffer *buffer) {
    destructBuffer(buffer);

    if (pthread_mutex_init(&buffer->mutex, NULL) != 0) {
        perror("pthread_mutex_init");
        exit(1);
    }
    buffer->empty = sem_open("/empty", O_CREAT, 0644, BUFFER_SIZE - 1);
    if (buffer->empty == SEM_FAILED) {
        perror("sem_open");
        exit(1);
    }
    buffer->full = sem_open("/full", O_CREAT, 0644, 0);
    if (buffer->full == SEM_FAILED) {
        perror("sem_open");
        exit(1);
    }
}

/** destructBuffer
 * Destroys the buffer and cleans up the mutex and semaphores
 * @param buffer - the buffer to destroy
 */
void destructBuffer(Buffer *buffer) {
    pthread_mutex_destroy(&buffer->mutex);
    sem_close(buffer->empty);
    sem_close(buffer->full);
    sem_unlink("/empty");
    sem_unlink("/full");

    buffer->head = 0;
    buffer->tail = 0;
}

/** enqueue
 * Adds an item to the buffer
 * Since the buffer is circular, the tail is incremented and wrapped around if it goes past the end of the buffer
 * @param buffer - the buffer to add the item to
 * @param item - the item to add
 * @return 0 if successful, -1 if the buffer is full
 */
int enqueue(Buffer *buffer, BufferItem item) {
    if (isFull(buffer)) {
        return -1;
    }
    buffer->items[buffer->tail] = item;
    buffer->tail = (buffer->tail + 1) % BUFFER_SIZE;
    return 0;
}

/** dequeue
 * Removes an item from the buffer
 * Since the buffer is circular, the head is incremented and wrapped around if it goes past the end of the buffer
 * @param buffer - the buffer to remove the item from
 * @param item - the item to remove
 * @return 0 if successful, -1 if the buffer is empty
 */
int dequeue(Buffer *buffer, BufferItem *item) {
    if (isEmpty(buffer)) {
        return -1;
    }
    *item = buffer->items[buffer->head];
    buffer->head = (buffer->head + 1) % BUFFER_SIZE;
    return 0;
}

/** isEmpty
 * Checks if the buffer is empty
 * @param buffer - the buffer to check
 * @return 1 if the buffer is empty, 0 otherwise
 */
int isEmpty(Buffer *buffer) {
    return buffer->head == buffer->tail;
}

/** isFull
 * Checks if the buffer is full
 * @param buffer - the buffer to check
 * @return 1 if the buffer is full, 0 otherwise
 */
int isFull(Buffer *buffer) {
    return (buffer->tail + 1) % BUFFER_SIZE == buffer->head;
}
