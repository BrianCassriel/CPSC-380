/** Schedule_RR.c
 * Schedules tasks in the order they arrive in.
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
 * Creates a new task and adds it to the list in fcfs order.
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

    insertAfter(taskList.tailNode->prev, task);
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
 * Picks the next task to run in fcfs order.
 * @return the next task to run
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
 * @param task - the task to check = 1 if the task has completed, 0 otherwise
 * @return 1 if the task has completed, 0 otherwise
 */
int isLastRun(Task *task) {
    return task->burstRemaining == 0;
}

/** runTask()
 * Runs the task for the duration of its burst time & removes it from list.
 * If the task does not complete in the time quantum, it is moved to the end of the list.
 * @param task - the task to run
 */
void runTask(Task* task) {
    int slice;
    if (isFirstRun(task))
        task->startTime = currTime;

    slice = task->burstRemaining > QUANTUM ? QUANTUM : task->burstRemaining;
    delete(&taskList.headNode, task);
    run(task, slice);
    task->burstRemaining -= slice;

    currTime += slice;
    if (isLastRun(task)) {
        task->exitTime = currTime;
    } else {
        insertAfter(taskList.tailNode->prev, task);
    }
}

/** schedule()
 * Schedules the tasks in fcfs order.
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