#include "image_rotation.h"
#include "queue.h"

#include <dirent.h>
#include <stdbool.h>

// TODO: Remove this for final submission
#define INTERMEDIATE_SUBMISSION

//Global integer to indicate the length of the queue??
//Global integer to indicate the number of worker threads
//Global file pointer for writing to log file in worker??
//Might be helpful to track the ID's of your threads in a global array
//What kind of locks will you need to make everything thread safe? [Hint you need multiple]
//What kind of CVs will you need  (i.e. queue full, queue empty) [Hint you need multiple]
//How will you track the requests globally between threads? How will you ensure this is thread safe?
//How will you track which index in the request queue to remove next?
//How will you update and utilize the current number of requests in the request queue?
//How will you track the p_thread's that you create for workers?
//How will you know where to insert the next request received into the request queue?



Worker* makeWorker(int id, ThreadSafeQueue *queue, LogFile *logFile, char *outputDirPath)
{
    Worker *worker = malloc(sizeof(Worker));

    worker->id = id;
    worker->queue = queue;
    worker->logFile = logFile;
    worker->imageProcessCount = 0;
    worker->done = false;
    worker->okToTerminate = false;
    worker->outputDirPath = outputDirPath;

    worker->lock = malloc(sizeof(pthread_mutex_t));
    worker->flagUpdateCond = malloc(sizeof(pthread_cond_t));
    
    pthread_mutex_init(worker->lock, NULL);
    pthread_cond_init(worker->flagUpdateCond, NULL);

    return worker;
}

/**
 * Frees any dynamic memory OWNED by the Worker struct's variables,
 * leaving the Worker struct itself ready to be freed
 */
void cleanupWorker(Worker* worker)
{
    free(worker->lock);
    free(worker->flagUpdateCond);
}


/*
    The Function takes:
    to_write: A file pointer of where to write the logs. 
    requestNumber: the request number that the thread just finished.
    file_name: the name of the file that just got processed. 

    The function output: 
    it should output the threadId, requestNumber, file_name into the logfile and stdout.
*/
void log_pretty_print(FILE* to_write, int threadId, int requestNumber, char * file_name){
   
}


/*

    1: The processing function takes a void* argument called args. It is expected to be a pointer to a structure processing_args_t 
    that contains information necessary for processing.

    2: The processing thread need to traverse a given dictionary and add its files into the shared queue while maintaining synchronization using lock and unlock. 

    3: The processing thread should pthread_cond_signal/broadcast once it finish the traversing to wake the worker up from their wait.

    4: The processing thread will block(pthread_cond_wait) for a condition variable until the workers are done with the processing of the requests and the queue is empty.

    5: The processing thread will cross check if the condition from step 4 is met and it will signal to the worker to exit and it will exit.

*/

void *processing(void *args)
{
    struct dirent *dirEntry;
    char *dirEntryPath;
    LinkedQueueElement *newElement;
    ProcessingArgs *pArgs = (ProcessingArgs*) args;

    int fileCounter = 0;

    DIR *dir = opendir(pArgs->dirPath);
    if (NULL == dir)
    {
        fprintf(stderr, "Failed to open directory %s\n", pArgs->dirPath);
    }
    else
    {
        while((dirEntry = readdir(dir)) != NULL)
        {
            // Process file in directory
            if (dirEntry->d_type == DT_REG)
            {
                // Concatanate the directory path with the file name
                dirEntryPath = malloc(strlen(pArgs->dirPath) + strlen(dirEntry->d_name) + 4);
                sprintf(dirEntryPath, "%s/%s", pArgs->dirPath, dirEntry->d_name);

                // Create a new queue element to hold this file path and it's rotation angle
                newElement = malloc(sizeof(LinkedQueueElement));
                newElement->file = dirEntryPath;
                newElement->rotationAngle = pArgs->angleOfRotation;

                // Add it to the queue (this function is thread-safe)
                TSQueue_add(pArgs->queue, newElement);
                fileCounter++;
            }
        }

        closedir(dir);

        // wait for ALL workers to finish
        for(int i = 0; i < pArgs->numWorkerThreads; i++)
        {
            pthread_mutex_lock(pArgs->workers[i]->lock);
            
            // Wait until the worker sets it's done flag to true
            while (!pArgs->workers[i]->done)
            {
                // If the worker is not done, wait for a signal from it until we re-check the flag
                pthread_cond_wait(pArgs->workers[i]->flagUpdateCond, pArgs->workers[i]->lock);
            }

            pthread_mutex_unlock(pArgs->workers[i]->lock);
        }

        // TODO: verify number of images processed by workers = number of elements we added to queue ("fileCounter")
        
        // Signal all workers to exit
        for(int i = 0; i < pArgs->numWorkerThreads; i++)
        {
            // Acquire this specific worker's lock, we are modifying data!
            pthread_mutex_lock(pArgs->workers[i]->lock);

            // Set the terminate flag to true, and then send a signal showing we updated it
            pArgs->workers[i]->okToTerminate = true;
            pthread_cond_broadcast(pArgs->workers[i]->flagUpdateCond);

            pthread_mutex_unlock(pArgs->workers[i]->lock);
        }

        printf("Processing Exit.\n");
    }

    return NULL;
}

/*
    1: The worker threads takes an int ID as a parameter

    2: The Worker thread will block(pthread_cond_wait) for a condition variable that there is a requests in the queue. 

    3: The Worker threads will also block(pthread_cond_wait) once the queue is empty and wait for a signal to either exit or do work.

    4: The Worker thread will processes request from the queue while maintaining synchronization using lock and unlock. 

    5: The worker thread will write the data back to the given output dir as passed in main. 

    6:  The Worker thread will log the request from the queue while maintaining synchronization using lock and unlock.  

    8: Hint the worker thread should be in a While(1) loop since a worker thread can process multiple requests and It will have two while loops in total
        that is just a recommendation feel free to implement it your way :) 
    9: You may need different lock depending on the job.  

*/


void * worker(void *args)
{

    Worker *thisWorker = (Worker*) args;

#ifndef INTERMEDIATE_SUBMISSION

        // TODO: Implement for final submission
        
        /*
            Stbi_load takes:
                A file name, int pointer for width, height, and bpp

        */

       // uint8_t* image_result = stbi_load("??????","?????", "?????", "???????",  CHANNEL_NUM);
        

        uint8_t **result_matrix = (uint8_t **)malloc(sizeof(uint8_t*) * width);
        uint8_t** img_matrix = (uint8_t **)malloc(sizeof(uint8_t*) * width);
        for(int i = 0; i < width; i++){
            result_matrix[i] = (uint8_t *)malloc(sizeof(uint8_t) * height);
            img_matrix[i] = (uint8_t *)malloc(sizeof(uint8_t) * height);
        }
        /*
        linear_to_image takes: 
            The image_result matrix from stbi_load
            An image matrix
            Width and height that were passed into stbi_load
        
        */
        //linear_to_image("??????", "????", "????", "????");
        

        ////TODO: you should be ready to call flip_left_to_right or flip_upside_down depends on the angle(Should just be 180 or 270)
        //both take image matrix from linear_to_image, and result_matrix to store data, and width and height.
        //Hint figure out which function you will call. 
        //flip_left_to_right(img_matrix, result_matrix, width, height); or flip_upside_down(img_matrix, result_matrix ,width, height);


        
        
        //uint8_t* img_array = NULL; ///Hint malloc using sizeof(uint8_t) * width * height
    

        ///TODO: you should be ready to call flatten_mat function, using result_matrix
        //img_arry and width and height; 
        //flatten_mat("??????", "??????", "????", "???????");


        ///TODO: You should be ready to call stbi_write_png using:
        //New path to where you wanna save the file,
        //Width
        //height
        //img_array
        //width*CHANNEL_NUM
       // stbi_write_png("??????", "?????", "??????", CHANNEL_NUM, "??????", "?????"*CHANNEL_NUM);
#else

    // For the intermediate submission just print the thread ID and exit
    printf("Worker Thread #%i\n", thisWorker->id);

#endif // INTERMEDIATE_SUBMISSION

    // Set done flag and send a signal
    pthread_mutex_lock(thisWorker->lock);
    thisWorker->done = true;
    pthread_cond_broadcast(thisWorker->flagUpdateCond);
    pthread_mutex_unlock(thisWorker->lock);

    // Wait for processing thread to tell us OK to exit
    pthread_mutex_lock(thisWorker->lock);
    while(!thisWorker->okToTerminate)
    {
        // Terminate flag not updated. Wait until we receive a signal from the processing thread to check again
        pthread_cond_wait(thisWorker->flagUpdateCond, thisWorker->lock);
    }
    pthread_mutex_unlock(thisWorker->lock);

    return NULL;
}

/*
    Main:
        Get the data you need from the command line argument 
        Open the logfile
        Create the threads needed
        Join on the created threads
        Clean any data if needed. 


*/

int main(int argc, char* argv[])
{
    int exitCode = 0;

    char *imageDirPath;
    char *outputDirPath;
    char *numWorkersStr;
    int numWorkers;
    char *rotationAngleStr;
    int rotationAngle;

    ThreadSafeQueue *queue;
    ProcessingArgs *processingArgs;
    Worker **workers;

    pthread_t processorTID;
    
    LogFile *logFile;

    if(argc != 5)
    {
        fprintf(stderr, "Usage: File Path to image dirctory, File path to output dirctory, number of worker thread, and Rotation angle\n");
        exitCode = 1;
    }
    else
    {
        // Load and convert arguments 
        imageDirPath = argv[1];
        outputDirPath = argv[2];
        numWorkersStr = argv[3];
        rotationAngleStr = argv[4];

        numWorkers = atoi(numWorkersStr);
        rotationAngle = atoi(rotationAngleStr);

        // Create the queue
        queue = createTSQueue();

        // Create LogFile struct
        logFile = malloc(sizeof(LogFile));
        logFile->file = fopen(LOG_FILE_NAME, "w");
        logFile->lock = malloc(sizeof(pthread_mutex_t));
        pthread_mutex_init(logFile->lock, NULL);
        
        workers = malloc(sizeof(pthread_t) * numWorkers);
      
        processingArgs = malloc(sizeof(ProcessingArgs));
        processingArgs->workers = workers;
        processingArgs->numWorkerThreads = numWorkers;
        processingArgs->angleOfRotation = rotationAngle;
        processingArgs->dirPath = imageDirPath;
        processingArgs->queue = queue;

        // Spawn processing thread
        pthread_create(&processorTID, NULL, processing, (void*) processingArgs);

        for (int i = 0; i < numWorkers; i++)
        {
            workers[i] = makeWorker(i, queue, logFile, outputDirPath);

            // Spawn worker thread
            pthread_create(&workers[i]->threadID, NULL, worker, (void*) workers[i]);
        }

        // Wait for all threads to finish
        for (int i = 0; i < numWorkers; i++)
        {
            pthread_join(workers[i]->threadID, NULL);
        }
        pthread_join(processorTID, NULL);

        // Free dynamic memory

        for (int i = 0; i < numWorkers; i++)
        {
            cleanupWorker(workers[i]);
            free(workers[i]);
        }
        free(workers);

        // Free queue related memory
        TSQueue_cleanup(&queue);

        free(processingArgs);
        
        // Close log file and free related memory
        fclose(logFile->file);
        free(logFile->lock);
        free(logFile);
    }
    
    return exitCode;
}
