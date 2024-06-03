/** cpu.h header
 * "Virtual" CPU that also maintains track of system time.
 */

#include "task.h"

#define QUANTUM 10 /// time slice for each task in a rr scheduler

/** run()
 * Simulate the execution of a task for a given slice of time.
 * @param task - the task to be executed
 * @param slice - the amount of time to execute the task
 */
void run(Task *task, int slice);
