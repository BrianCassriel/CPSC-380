/** schedulers.h header
 * This file contains the function prototypes for the scheduler algorithms.
 * It also contains some implementation details that are shared between the scheduler algorithms.
 */

#ifndef SCHEDULERS_H
#define SCHEDULERS_H

#include <stdlib.h>
#include <stdio.h>

#include "cpu.h"
#include "list.h"

#define MIN_PRIORITY 1
#define MAX_PRIORITY 10
#define ARRIVAL_TIME 0

void initList();
void freeList();
void add(char *name, int priority, int burst);
void priorityInsert(Task *nextTask);
void updateAverageTimes(Task *task);
Task* pickNextTask();
int isFirstRun(Task *task);
int isLastRun(Task *task);
void runTask(Task *task);
void schedule();

#endif
