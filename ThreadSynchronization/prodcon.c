// prodcon.c implementation
// Created by Brian Cassriel on 4/7/24.
// Contains the main method for the producer consumer problem.
// This file acts as the driver for all threads and interfaces with the buffer.

#include <unistd.h>
#include <time.h>
#include "buffer.h"
#include "Checksum.h"

Buffer buffer; /// global buffer

/** insert_item
 * Inserts an item into the global buffer
 * @param item - the item to insert
 * @return - 0 if successful, -1 if failed
 */
int insert_item(BufferItem item) {
    int result;
    sem_wait(buffer.empty);
    pthread_mutex_lock(&buffer.mutex);
    result = enqueue(&buffer, item);
    pthread_mutex_unlock(&buffer.mutex);
    sem_post(buffer.full);
    return result;
}

/** remove_item
 * Removes an item from the global buffer
 * @param item - the item to remove
 * @return - 0 if successful, -1 if failed
 */
int remove_item(BufferItem *item) {
    int result;
    sem_wait(buffer.full);
    pthread_mutex_lock(&buffer.mutex);
    result = dequeue(&buffer, item);
    pthread_mutex_unlock(&buffer.mutex);
    sem_post(buffer.empty);
    return result;
}

/** createItem
 * Creates a new buffer item with random data and a checksum
 * @return - the new buffer item
 */
BufferItem createItem() {
    BufferItem item;
    for (int i = 0; i < 30; i++) {
        item.data[i] = (uint8_t) (rand() % 10);
    }
    item.checksum = checksum((char*) item.data, 30);
    return item;
}

/** checksumNotValid
 * Checks if the checksum of a buffer item is valid
 * @param item - the buffer item to check
 * @return - 1 if the checksum is not valid, 0 if it is
 */
int checksumNotValid(BufferItem *item) {
    return (*item).checksum != checksum((char*) (*item).data, 30);
}

/** producer
 * Producer thread function that attempts to insert items into the buffer at random intervals
 */
void *producer (void *param) {
    BufferItem item;
    while (1) {
        sleep(rand() % 4);
        item = createItem();
        if (insert_item(item))
            printf("producer errored while inserting item\n");
        else
            printf("producer produced %u (+)\n", item.checksum);
    }
}

/** consumer
 * Consumer thread function that attempts to remove items from the buffer at random intervals
 */
void *consumer (void *param) {
    BufferItem item;
    while (1) {
        sleep(rand() % 4);
        if (remove_item(&item))
            printf("consumer errored while removing item\n");
        else {
            printf("consumer consumed %u (-)\n", item.checksum);
            if (checksumNotValid(&item)) { // this should never be hit since we are using a buffer and not actually sending data
                printf("consumer detected error in checksum. expected: %d, calculated: %d\n", item.checksum,
                       checksum((char*) item.data, 30));
                exit(1);
            }
        }
    }
}

/** createProdThreads
 * Creates a specified number of producer threads
 * @param numProducer - the number of producer threads to create
 * @param prod_tids - the array to store the thread ids
 */
void createProdThreads(int numProducer, pthread_t* prod_tids) {
    for (int i = 0; i < numProducer; i++) {
        pthread_t tid;
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        pthread_create(&tid, &attr, producer, NULL);
        if (tid == 0) {
            printf("Error creating producer thread\n");
            exit(1);
        }
        prod_tids[i] = tid;
    }
}

/** createConsThreads
 * Creates a specified number of consumer threads
 * @param numConsumer - the number of consumer threads to create
 * @param cons_tids - the array to store the thread ids
 */
void createConsThreads(int numConsumer, pthread_t* cons_tids) {
    for (int i = 0; i < numConsumer; i++) {
        pthread_t tid;
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        pthread_create(&tid, &attr, consumer, NULL);
        if (tid == 0) {
            printf("Error creating consumer thread\n");
            exit(1);
        }
        cons_tids[i] = tid;
    }
}

/** main
 * Main method that creates producer and consumer threads and runs them for a specified amount of time
 * @param argc - the number of arguments
 * @param argv - the arguments
 * @return - 0 if successful
 */
int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Usage: ./prodcon <delay> <numProducer> <numConsumer>\n");
        return -1;
    }

    int delay;
    int numProducer;
    int numConsumer;

    delay = atoi(argv[1]);
    numProducer = atoi(argv[2]);
    numConsumer = atoi(argv[3]);
    initBuffer(&buffer);
    srand(time(NULL));

    pthread_t prod_tids[numProducer];
    pthread_t cons_tids[numConsumer];
    createProdThreads(numProducer, prod_tids);
    createConsThreads(numConsumer, cons_tids);

    sleep(delay);

    for (int i = 0; i < numProducer; i++)
        pthread_cancel(prod_tids[i]);
    for (int i = 0; i < numConsumer; i++)
        pthread_cancel(cons_tids[i]);
    destructBuffer(&buffer);
    return 0;
}
