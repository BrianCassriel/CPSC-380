### Brian Cassriel
2422454 | cassriel@chapman.edu

CPSC380-02 | **Programming Assignment 3: CPU Scheduling**

Source Files: driver.c CPU.c cpu.h list.c list.h task.h schedulers.h schedule_fcfs.c schedule_sjf.c schedule_rr.c schedule_priority.c schedule_priority_rr.c Makefile schedule.txt book.txt pri-schedule.txt rr-schedule.txt

No errors noted.

Sources Cited:
```
For info on arrival time and exit time: https://afteracademy.com/blog/what-is-burst-arrival-exit-response-waiting-turnaround-time-and-throughput/
```

Use Makefile to compile each type of scheduling algorithm. For example, use `make fcfs` to compile the first-come-first-serve scheduling algorithm. Besides `fcfs`, there is also `sjf`, `rr`, `priority`, and `priority_rr`.

Use `./<algorithm type> <schedule textfile>` to run each type of scheduling algorthm.
Example: `./fcfs schedule.txt`.
```
Running task = [T1] [4] [20] for 20 units.
Running task = [T2] [3] [25] for 25 units.
Running task = [T3] [3] [25] for 25 units.
Running task = [T4] [5] [15] for 15 units.
Running task = [T5] [5] [20] for 20 units.
Running task = [T6] [1] [10] for 10 units.
Running task = [T7] [3] [30] for 30 units.
Running task = [T8] [10] [25] for 25 units.
Average Turnaround Time: 94.38
Average Waiting Time: 73.12
Average Response Time: 73.12
```
