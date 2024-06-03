/** Schedule_Priority_RR.c
 * Schedules tasks in order of priority. (Highest priority first)
 * If a task does not complete in the time quantum, it is moved to the end of the list.
 */

#include "schedulers.h"

int tid = 0;
List taskList;
int currTime = 0;
int turnaroundTimeSum = 0;
int waitingTimeSum = 0;
int responseTimeSum = 0;

/** initList()
 * Initializes the list by creating a head and a tail node.
 */
void initList() {
    struct node *head = malloc(sizeof(struct node));
    struct node *tail = malloc(sizeof(struct node));
    head->next = tail;
    head->prev = NULL;
    tail->next = NULL;
    tail->prev = head;

    taskList.headNode = head;
    taskList.tailNode = tail;
}

/** freeList()
 * Frees the memory allocated for the list.
 */
void freeList() {
    struct node *temp;
    struct node *next;

    temp = taskList.headNode;
    while (temp != NULL) {
        next = temp->next;
        free(temp);
        temp = next;
    }
}

/** add()
 * Creates a new task and adds it to the list in order of priority.
 * @param name - name of the task
 * @param priority - priority of the task (higher number = higher priority)
 * @param burst - burst time of the task
 */
void add(char *name, int priority, int burst) {
    Task *task = malloc(sizeof(Task));
    task->name = name;
    task->tid = tid;
    task->priority = priority;
    task->burst = burst;
    task->burstRemaining = burst;
    task->startTime = -1;
    task->exitTime = -1;
    __sync_fetch_and_add(&tid, 1); // tid++;

    priorityInsert(task);
}

/** priorityInsert()
 * Inserts a task into the list in order of priority.
 * If there are multiple tasks with the same priority, they are inserted in fcfs order.
 * @param nextTask - the task to insert
 */
void priorityInsert(Task *task) {
    struct node *temp = taskList.headNode;
    while (temp->next != taskList.tailNode && task->priority <= temp->next->task->priority) {
        temp = temp->next;
    }
    insertAfter(temp, task);
}

/** updateAverageTimes()
 * Updates the average turnaround, waiting, and response times.
 * @param task - the task to update the times for
 */
void updateAverageTimes(Task *task) {
    turnaroundTimeSum += task->exitTime - ARRIVAL_TIME;
    waitingTimeSum += (task->exitTime - ARRIVAL_TIME) - task->burst;
    responseTimeSum += task->startTime - ARRIVAL_TIME;
}

/** pickNextTask()
 * Picks the task with the highest priority to run next.
 * @return the task with the highest priority
 */
Task* pickNextTask() {
    return taskList.headNode->next->task;
}

/** isFirstRun()
 * Checks if the task has not been run yet.
 * @param task - the task to check
 * @return 1 if the task has not been run, 0 otherwise
 */
int isFirstRun(Task *task) {
    return task->startTime == -1;
}

/** isLastRun()
 * Checks if the task has completed its burst time.
 * @param task - the task to check
 * @return 1 if the task has completed, 0 otherwise
 */
int isLastRun(Task *task) {
    return task->burstRemaining == 0;
}

/** runTask()
 * Runs the task for the duration of its burst time & removes it from list.
 * If the task does not complete in the time quantum, it is put back in the list in priority order.
 * @param task - the task to run
 */
void runTask(Task* task) {
    int slice;

    if (isFirstRun(task))
        task->startTime = currTime;

    slice = task->burstRemaining > QUANTUM ? QUANTUM : task->burstRemaining;
    delete(&taskList.headNode, taskList.headNode->next->task);
    run(task, slice);
    task->burstRemaining -= slice;

    currTime += slice;
    if (isLastRun(task)) {
        task->exitTime = currTime;
    } else {
        priorityInsert(task);
    }
}

/** schedule()
 * Schedules the tasks in order of priority.
 * Prints the average turnaround, waiting, and response times.
 */
void schedule() {
    Task* nextTask;
    int totalTasks;

    while (taskList.headNode->next != taskList.tailNode) {
        nextTask = pickNextTask();
        runTask(nextTask);
        if (isLastRun(nextTask)) {
            updateAverageTimes(nextTask);
            free(nextTask);
        }
    }

    totalTasks = tid;
    printf("Average Turnaround Time: %.2f\n", (double)turnaroundTimeSum / totalTasks);
    printf("Average Waiting Time: %.2f\n", (double)waitingTimeSum / totalTasks);
    printf("Average Response Time: %.2f\n", (double)responseTimeSum / totalTasks);

    freeList();
}