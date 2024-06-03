/** Driver.c
 * This file is the driver for the scheduler.
 *
 * Schedule is printed in the format
 * [name] [priority] [CPU burst]
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "schedulers.h"

#define SIZE 100

/** Main method
 * Reads in a file of tasks and adds them to the scheduler's list of tasks.
 * @param argc - number of arguments
 * @param argv - array of arguments. Usage: ./<scheduler> <schedule textfile>
 * @return - 0 if successful, 1 if file could not be opened
 */
int main(int argc, char *argv[])
{
    FILE *in;
    char *temp;
    char task[SIZE];

    char *name;
    int priority;
    int burst;
    
    in = fopen(argv[1],"r");
    if (in == NULL) {
        printf("Could not open file. Usage: ./<scheduler> <schedule textfile>\n");
        return 1;
    }

    initList();
    while (fgets(task,SIZE,in) != NULL) {
        temp = strdup(task);
        name = strsep(&temp,",");
        priority = atoi(strsep(&temp,","));
        burst = atoi(strsep(&temp,","));

        add(name,priority,burst);

        free(temp);
    }

    fclose(in);

    schedule();

    return 0;
}
