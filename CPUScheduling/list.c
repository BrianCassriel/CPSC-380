/** list.c implementation
 * Defines various list operations.
 */
 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "list.h"
#include "task.h"

/** insertAfter()
 * Inserts a new task into the list after the given node.
 * @param node - the node to insert after
 * @param newTask - the task to insert
 */
void insertAfter(struct node *node, Task *newTask) {
    struct node *newNode = malloc(sizeof(struct node));
    newNode->task = newTask;

    newNode->next = node->next;
    newNode->prev = node;
    node->next->prev = newNode;
    node->next = newNode;
}

/** delete()
 * Deletes a task from the list.
 * @param head - the head of the list
 * @param task - the task to delete
 */
void delete(struct node **head, Task *task) {
    struct node *temp;
    struct node *prev;

    temp = (*head)->next;
    // special case - beginning of list
    if (strcmp(task->name,temp->task->name) == 0) {
        *head = (*head)->next;
        (*head)->prev = NULL;
        (*head)->task = NULL;
    } else {
        // interior or last element in the list
        prev = *head;
        temp = temp->next;
        while (strcmp(task->name,temp->task->name) != 0) {
            prev = temp;
            temp = temp->next;
        }

        prev->next = temp->next;
    }
}

/** traverse()
 * Traverses the list and prints the tasks.
 * @param head - the head of the list
 */
void traverse(struct node *head) {
    struct node *temp;
    temp = head->next;

    while (temp->next != NULL) {
        printf("[%s] [%d] [%d]\n",temp->task->name, temp->task->priority, temp->task->burst);
        temp = temp->next;
    }
    fflush(stdout);
}
