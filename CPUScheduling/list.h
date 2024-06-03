/** list.h header
 * doubly linked list data structure containing the tasks for the scheduler to use
 */

#include "task.h"

/** node
 * A node in the list
 * @param task - the task
 * @param next - the next node
 * @param prev - the previous node
 */
struct node {
    Task *task;
    struct node *next;
    struct node *prev;
};

/** List
 * A list of nodes
 * @param headNode - the empty head node
 * @param tailNode - the empty tail node
 */
typedef struct list {
    struct node *headNode;
    struct node *tailNode;
} List;

void insertAfter(struct node *node, Task *task);
void delete(struct node **head, Task *task);
void traverse(struct node *head);
