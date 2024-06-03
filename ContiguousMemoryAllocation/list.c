// list.c
// Created by Brian Cassriel on 4/15/24.
// This file contains useful functions for the doubly linked list data structure.

#include "list.h"
#include "memory.h"

/** initList
 * Initializes the list with an empty head and tail node.
 * @param list - the list to initialize
 */
void initList(List *list) {
    list->headNode = malloc(sizeof(struct node));
    list->tailNode = malloc(sizeof(struct node));

    list->headNode->next = list->tailNode;
    list->headNode->prev = NULL;
    list->tailNode->next = NULL;
    list->tailNode->prev = list->headNode;
}

/** insertAfter
 * Inserts a new block after the given index.
 * @param list - the list to insert into
 * @param newBlock - the block to insert
 * @param index - the index to insert after
 */
void insertAfter(List* list, Block *newBlock, int index) {
    struct node *newNode = malloc(sizeof(struct node));
    newNode->block = newBlock;

    struct node *temp = list->headNode->next;
    int i = 0;
    while (temp->next != NULL) {
        if (i == index) {
            newNode->next = temp->next;
            newNode->prev = temp;
            temp->next->prev = newNode;
            temp->next = newNode;
            return;
        }
        i++;
        temp = temp->next;
    }
}

/** append
 * Appends a new block to the end of the list.
 * @param list - the list to append to
 * @param newBlock - the block to append
 */
void append(List *list, Block *newBlock) {
    struct node *newNode = malloc(sizeof(struct node));

    newNode->block = newBlock;
    newNode->next = list->tailNode;
    newNode->prev = list->tailNode->prev;
    list->tailNode->prev->next = newNode;
    list->tailNode->prev = newNode;
}

/** prepend
 * Prepends a new block to the beginning of the list.
 * @param list - the list to prepend to
 * @param newBlock - the block to prepend
 */
void prepend(List *list, Block *newBlock) {
    struct node *newNode = malloc(sizeof(struct node));

    newNode->block = newBlock;
    newNode->next = list->headNode->next;
    newNode->prev = list->headNode;
    list->headNode->next->prev = newNode;
    list->headNode->next = newNode;
}

/** delete
 * Deletes a block from the list at the given index.
 * The block is freed from memory.
 * @param list - the list to delete from
 * @param index - the index to delete
 */
void delete(List *list, int index) {
    struct node *temp;
    struct node *prev;
    int i = 0;

    temp = list->headNode->next;
    while (temp->next != NULL) {
        if (i == index) {
            prev = temp->prev;
            prev->next = temp->next;
            temp->next->prev = prev;
            free(temp);
            return;
        }
        i++;
        temp = temp->next;
    }
}

/** deallocate
 * Deallocates the memory used by the list.
 * @param list - the list to deallocate
 */
void deallocate(List *list) {
    struct node *temp;
    struct node *next;

    temp = list->headNode->next;
    while (temp->next != NULL) {
        next = temp->next;
        free(temp);
        temp = next;
    }
    free(list->headNode);
}

/** getSize
 * Gets the size of the list.
 * @param list - the list to get the size of
 * @return the size of the list
 */
int getSize(List *list) {
    struct node *temp;
    int size = 0;

    temp = list->headNode->next;
    while (temp->next != NULL) {
        size++;
        temp = temp->next;
    }

    return size;
}

/** getBlock
 * Gets the block at the given index.
 * @param list - the list to get the block from
 * @param index - the index of the block
 * @return the block at the given index
 */
Block* getBlock(List *list, int index) {
    struct node *temp;
    int i = 0;

    temp = list->headNode->next;
    while (temp->next != NULL) {
        if (i == index) {
            return temp->block;
        }
        i++;
        temp = temp->next;
    }

    return temp->block;
}

/** findBlock
 * Finds a block in the list with the given name.
 * @param list - the list to search
 * @param name - the name of the block to find
 * @return the block with the given name
 */
Block* findBlock(List *list, char* name) {
    struct node *temp;
    temp = list->headNode->next;

    while (temp->next != NULL) {
        if (strcmp(name,temp->block->name) == 0) {
            return temp->block;
        }
        temp = temp->next;
    }

    return NULL;
}
