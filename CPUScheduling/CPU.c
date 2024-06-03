/** CPU.c implementation
 * "Virtual" CPU that also maintains track of system time.
 */

#include <stdio.h>

#include "cpu.h"

/** run()
 * Simulate the execution of a task for a given slice of time.
 * @param task - the task to be executed
 * @param slice - the amount of time to execute the task
 */
void run(Task *task, int slice) {
    printf("Running task = [%s] [%d] [%d] for %d units.\n",task->name, task->priority, task->burst, slice);
}
