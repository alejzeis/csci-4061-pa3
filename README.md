# csci-4061-pa3

**Project Group**: 133

**Group Members**:
- Alejandro Zeise (zeise035)
- Charlie Youngren (youn2835)

**Device Tested On**: Personal Laptop, running Fedora 38 (Linux 6.4.15, GCC 13.2.1)

**Existing File Changes**:
- **Makefile:** Added additional source file to compile `queue.h`

**Contributions:**
- Alejandro: `image_rotation.c` (processing thread/main), `queue.c/h`
- Charlie: `image_rotation.c`

### Plan
A `Worker` struct is created for every worker that needs to be spawned.
The struct contains pointers to all the data the worker thread needs to execute
it's task, notably a pointer to the Queue data structure.

The `ThreadSafeQueue` implementation is thread safe, it's add and pop functions
will automatically lock and unlock it's associated mutex as necessary, broadcasting
a signal on the elementAddedCondition variable when a new element is added.

Worker threads will have the following general structure (Pseudocode),
```
do
{
    worker.mutex.unlock() // while loop conditional check finished, unlock mutex

    while ((element = worker.queue.pop()) != NULL)
    {
        // Print the request stuff to the log file
        worker.logFile.mutex.lock();
        log_pretty_print();
        worker.logFile.mutex.unlock();

        // doImageRotation here with the popped element's data
    }

    // Inner while loop has exited, meaning queue is empty. Set done flag 

    worker.mutex.lock(); // Lock to modify done flag and check okToTerminate

    worker.done = true;

    ptrhead_cond_broadcast(worker.flagUpdateCond); // Broadcast on flag update condition variable that we changed our flag

} while (!worker.okToTerminate);
```

