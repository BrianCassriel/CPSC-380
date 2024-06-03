### Brian Cassriel
2422454 | cassriel@chapman.edu

CPSC380-02 | **Programming Assignment 4: Thread Synchronization**

Source Files: prodcon.c, buffer.c, buffer.h, Checksum.c, Checksum.h

Errors noted: No errors to note.

Sources Cited:
```
Canvas Resources: Thread Synchronization Assignment for Checksum.c.
OpenAI ChatGPT-3.5: Help with the circular buffer implementation.
```

`gcc -o prodcon *.c` to compile and `./prodcon <delay> <# producer threads> <# consumer threads>` to run.

Example output: `./prodcon 2 5 5`
```
producer produced 49325 (+)
producer produced 48327 (+)
consumer consumed 49325 (-)
producer produced 45749 (+)
producer produced 44470 (+)
consumer consumed 48327 (-)
producer produced 44221 (+)
consumer consumed 44470 (-)
consumer consumed 45749 (-)
consumer consumed 44221 (-)
producer produced 44490 (+)
consumer consumed 44490 (-)
```
