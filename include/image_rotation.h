#ifndef IMAGE_ROTATION_H_
#define IMAGE_ROTATION_H_

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <pthread.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <fcntl.h>
#include <sys/time.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>
#include <signal.h>
#include <limits.h>
#include <stdint.h>
#include "utils.h"

#include "queue.h"

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define CHANNEL_NUM 1

#include "stb_image.h"
#include "stb_image_write.h"



/********************* [ Helpful Macro Definitions ] **********************/
#define BUFF_SIZE 1024 
#define LOG_FILE_NAME "request_log"               //Standardized log file name
#define INVALID -1                                  //Reusable int for marking things as invalid or incorrect
#define MAX_THREADS 100                             //Maximum number of threads
#define MAX_QUEUE_LEN 100                           //Maximum queue length



/********************* [ Helpful Typedefs        ] ************************/

typedef struct LogFile_T
{
    /** File handle for the LogFile. Opened and closed in main() */
    FILE *file;

    /** Mutex for the file handle. Only write to file when this is locked */
    pthread_mutex_t *lock;
} LogFile;

typedef struct Worker_T
{
    pthread_t threadID;
    int id;

    char *outputDirPath; // Not owned by worker

    ThreadSafeQueue *queue; // pointer, not owned by Worker

    LogFile *logFile; // pointer, not owned by Worker

    /** Mutex for the worker */
    pthread_mutex_t *lock;

    int imageProcessCount;

    /** Set by the worker thread once it has finished processing all requests */
    bool done;
    /** Set by the processing thread when the Worker is allowed to terminate */
    bool okToTerminate;

    // Condition used to signal between threads when the "done" or "okToTerminate" flags have been updated.
    pthread_cond_t *flagUpdateCond;
} Worker;

typedef struct ProcessingArgs_T
{
    char *dirPath;
    int numWorkerThreads;
    int angleOfRotation;

    ThreadSafeQueue *queue; // Pointer, not owned by us

    Worker **workers;
} ProcessingArgs;

/********************* [ Function Prototypes       ] **********************/
Worker* makeWorker(int id, ThreadSafeQueue *queue, LogFile *logFile, char *outputDirPath);
void cleanupWorker(Worker* worker);

void *processing(void *args); 
void * worker(void *args); 
void log_pretty_print(FILE* to_write, int threadId, int requestNumber, char * file_name);

#endif
