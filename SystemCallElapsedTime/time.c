/**
 * This program measures the time (in microseconds) it takes to execute a command.
 * It uses shared memory to communicate the start and end times between the parent and child processes.
 * Usage: ./time <command>
 * Example: ./time ls -la
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>


/** openSharedMemory
 * This method opens a shared memory segment and includes some error handling.
 * @param sharedMemoryFile the file destination of the shared memory segment (pass by reference)
 * @param sharedMemoryName the name of the shared memory segment
 * @return 0 if the shared memory segment is opened successfully, 1 otherwise
 */
int openSharedMemory(int* sharedMemoryFile, char* sharedMemoryName) {
    *sharedMemoryFile = shm_open(sharedMemoryName, O_CREAT | O_RDWR, 0644);
    if (*sharedMemoryFile == -1) {
        fprintf(stderr, "Error unable to create shared memory, %s, errno = %d (%s)\n", sharedMemoryName, errno, strerror(errno));
        return 1;
    }
    return 0;
}

/** setSharedMemorySize
 * This method sets the size of a shared memory segment and includes some error handling.
 * @param sharedMemoryFile the file destination of the shared memory segment
 * @param sharedMemoryName the name of the shared memory segment
 * @return 0 if the shared memory segment is configured successfully, 1 otherwise
 */
int setSharedMemorySize(int sharedMemoryFile, char* sharedMemoryName) {
    if (ftruncate(sharedMemoryFile, sizeof(struct timeval)) == -1) {
        fprintf(stderr, "Error configure create shared memory, %s, errno = %d (%s)\n", sharedMemoryName, errno, strerror(errno));
        shm_unlink(sharedMemoryName);
        return 1;
    }
    return 0;
}

/** getSharedMemoryConfig
 * This method gets the configuration of a shared memory segment and includes some error handling.
 * @param sharedMemoryFile the file destination of the shared memory segment
 * @param sharedMemoryBuffer the buffer of the shared memory segment (pass by reference)
 * @return 0 if the shared memory segment is configured successfully, 1 otherwise
 */
int getSharedMemoryConfig(int sharedMemoryFile, struct stat* sharedMemoryBuffer) {
    if (fstat(sharedMemoryFile, sharedMemoryBuffer) == -1) {
        fprintf(stderr, "Error unable to status shared memory segment ID = %d, errno = %d (%s)\n", sharedMemoryFile, errno, strerror(errno));
        return 1;
    }
    return 0;
}

/** attachToSharedMemory
 * This method attaches to a shared memory segment and includes some error handling.
 * @param sharedMemoryPtr a pointer to the shared memory segment (pass by reference)
 * @param sharedMemoryFile the file destination of the shared memory segment
 * @param sharedMemoryBuffer the buffer of the shared memory segment
 * @return 0 if the shared memory segment is attached successfully, 1 otherwise
 */
int attachToSharedMemory(uint8_t*** sharedMemoryPtr, int sharedMemoryFile, struct stat sharedMemoryBuffer) {
    **sharedMemoryPtr = (uint8_t*) mmap(0, sharedMemoryBuffer.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, sharedMemoryFile, 0);
    if (**sharedMemoryPtr == MAP_FAILED) {
        fprintf(stderr, "Error: unable to map shared memory segment, errno = %d (%s) \n", errno, strerror(errno));
        return 1;
    }
    return 0;
}

/** removeSharedMemory
 * This method removes a shared memory segment with a given name. It also includes some error handling.
 * @param sharedMemoryName the name of the shared memory segment
 * @return 0 if the shared memory segment is removed successfully, 1 otherwise
 */
int removeSharedMemory(char* sharedMemoryName) {
    if (shm_unlink(sharedMemoryName) == -1) {
        fprintf(stderr, "Error unable to remove shared memory segment '%s', errno = %d (%s) \n", sharedMemoryName, errno, strerror(errno));
        return 1;
    }
    return 0;
}

/** createSharedMemory
 * This method creates, sets up, and attaches to a shared memory segment with a given name and size.
 * If any of the steps fail, it removes the shared memory segment.
 * @param sharedMemoryName the name of the shared memory segment
 * @param sharedMemoryPtr a pointer to the shared memory segment (pass by reference)
 * @return 0 if the shared memory segment is created successfully, 1 otherwise
 */
int createSharedMemory(char* sharedMemoryName, uint8_t** sharedMemoryPtr) {
    int sharedMemoryFile;
    struct stat sharedMemoryBuffer;

    if (openSharedMemory(&sharedMemoryFile, sharedMemoryName) != 1
        && setSharedMemorySize(sharedMemoryFile, sharedMemoryName) != 1
        && getSharedMemoryConfig(sharedMemoryFile, &sharedMemoryBuffer) != 1
        && attachToSharedMemory(&sharedMemoryPtr, sharedMemoryFile, sharedMemoryBuffer) != 1) {
        return 0;
    }
    removeSharedMemory(sharedMemoryName);
    return 1;
}

/** waitForChildToFinish
 * This method waits for the child process to finish and prints a message if the child exited abnormally.
 */
void waitForChildToFinish() {
    int exitStatus;

    wait(&exitStatus);
    if (!WIFEXITED(exitStatus)) {
        printf("Child exited abnormally\n");
    }
}

/** Main method
 * This method measures the time it takes to execute a command.
 * After creating an IPC shared memory segment, it forks a child process to execute the command.
 * Then, the parent process waits for the child to finish and prints the elapsed time.
 * It makes sure to remove the shared memory segment before exiting.
 * @param argc the number of arguments
 * @param argv the arguments
 * @return 0 if the program runs successfully, 1 otherwise
 */
int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage: %s <command>\n", argv[0]);
        return 1;
    }

    char* commandName = argv[1];
    char* commandArgs[argc - 1];
    struct timeval startTime, endTime;
    char* sharedMemoryName = "SystemCallElapsedTime";
    uint8_t* sharedMemoryPtr = NULL;

    int i;
    for (i = 2; i < argc; i++) {
        commandArgs[i - 2] = argv[i];
    }
    commandArgs[i - 1] = NULL; // null-terminate the commandArgs array
    if (createSharedMemory(sharedMemoryName, &sharedMemoryPtr) == 1) {
        return 1;
    }

    pid_t pid = fork();
    switch(pid)
    {
        case -1:
            fprintf(stderr, "Fork failed\n");
            return 1;
        case 0: // child
            gettimeofday(&startTime, NULL);
            memcpy(&sharedMemoryPtr[0], &startTime, sizeof(struct timeval));
            if (commandArgs[0] == NULL) {
                execlp(commandName, commandName, NULL);
            } else {
                execvp(commandName, commandArgs);
            }
            if (errno == 2) {
                fprintf(stderr, "Error: command not found\n");
            } else {
                fprintf(stderr, "Error: unable to execute command, errno = %d (%s)\n", errno, strerror(errno));
            }
            exit(0);
        default: // parent
            waitForChildToFinish();
            break;
    }
    gettimeofday(&endTime, NULL);
    memcpy(&startTime, &sharedMemoryPtr[0], sizeof(struct timeval));
    printf("\nElapsed Time for %s: %d usecs\n", commandName, endTime.tv_usec - startTime.tv_usec);
    if (removeSharedMemory(sharedMemoryName) == 1) {
        return 1;
    }
    return 0;
}
