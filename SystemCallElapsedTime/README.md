### Brian Cassriel
2422454 | cassriel@chapman.edu

CPSC380-02 | **Programming Assignment 1: System Call Elapsed Time**

Source Files: time.c

Errors noted:
Sometimes the terminal output from the timed command does not display, but the time seems to be accurate.

Sources Cited:
```
Canvas Resources: POSIX IPC and system calls
learn.microsoft.com: General usage and syntax of C and system calls (ex. execvp)
```

`gcc -o time time.c` to compile and `./time <command>` to run.

Example: `./time ps` to run the time command on the `ps` command.
```
  PID TTY           TIME CMD
68767 ttys000    0:00.06 -zsh
68906 ttys000    0:00.00 ./time ps

Elapsed Time for ps: 16534 usecs
```
