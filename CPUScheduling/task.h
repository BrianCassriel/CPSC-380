/** task.h
 * Representation of a task in the system.
 */

#ifndef TASK_H
#define TASK_H

/** Task
 * A task represents a process to run on the CPU.
 * This structure keeps track of things like the task's name, priority, burst time, etc.
 */
typedef struct task {
    char *name;
    int tid;
    int priority;
    int burst;
    int burstRemaining;
    int startTime;
    int exitTime;
} Task;

#endif
