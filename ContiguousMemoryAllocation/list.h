// list.h
// Created by Brian Cassriel on 4/15/24.
// This file contains the doubly linked list data structure and the function prototypes for list.c.

#ifndef LIST_H
#define LIST_H

typedef struct block Block;

struct node {
    Block *block;
    struct node *next;
    struct node *prev;
};

typedef struct list {
    struct node *headNode;
    struct node *tailNode;
} List;

void initList(List *list);
void insertAfter(List *list, Block *block, int index);
void append(List *list, Block *block);
void prepend(List *list, Block *block);
void delete(List *list, int index);
void deallocate(List *list);
int getSize(List *list);
Block* getBlock(List *list, int index);
Block* findBlock(List *list, char* name);

#endif
