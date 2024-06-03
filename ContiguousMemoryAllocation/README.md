### Brian Cassriel
2422454 | cassriel@chapman.edu

CPSC380-02 | **Programming Assignment 5: Contiguous Memory Allocation**

Source Files: allocator.c, memory.h, memory.c, list.h, list.c

No errors noted.

Sources Cited:
```
OpenAI ChatGPT-3.5: Help creating list.h and list.c
```

### Instructions
1. Compile the program using the following command: `gcc -o allocator *.c`
2. Run the program using the following command: `./allocator <memory_size>`
3. Use the commands `RQ`, `RL`, `C`, and `STAT` to interact with the program, or enter `X` to exit the program.

RQ Usage: `RQ <process_id> <memory_size>`

RL Usage: `RL <process_id>`

C Usage: `C`

STAT Usage: `STAT`

### Example output
```
allocator> RQ P0 1000 F
allocator> STAT
Allocated Memory: 10000 (0-9999)
Processes:
    P0: 1000 (0-999)
    Unused: 9000 (1000-9999)
allocator> RQ P1 5000 B
allocator> STAT
Allocated Memory: 10000 (0-9999)
Processes:
    P0: 1000 (0-999)
    P1: 5000 (1000-5999)
    Unused: 4000 (6000-9999)
allocator> RQ P2 4000 W
allocator> STAT
Allocated Memory: 10000 (0-9999)
Processes:
    P0: 1000 (0-999)
    P1: 5000 (1000-5999)
    P2: 4000 (6000-9999)
allocator> X

Process finished with exit code 0
```
