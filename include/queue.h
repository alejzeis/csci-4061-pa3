/**
 * queue.h
 *
 * Contains definitions for a Thread Safe Queue implementation.
 *
 * @author Alejandro Zeise
 */
#ifndef _QUEUE_H
#define _QUEUE_H

#include <stdbool.h>
#include <pthread.h>

/**
 * Struct for a single element in the Queue.
 * Contains the file path and rotation angle.
 */
typedef struct LinkedQueueElement_T
{
    char *file;
    int rotationAngle;

    /** Pointer to the next queue element */
    struct LinkedQueueElement_T *next;

    /** Pointer to the previous queue element */
    struct LinkedQueueElement_T *previous;
} LinkedQueueElement;

/**
 * Struct to hold data for a Thread Safe Queue.
 * The queue is implemented using a linked list.
 */
typedef struct ThreadSafeQueue_T
{
   LinkedQueueElement *head;
   LinkedQueueElement *tail;

   /** Mutex for the queue. Anytime the queue is modified (add/pop) this mutex is locked. */
   pthread_mutex_t *lock;

   /**
    * Flag set by the processing thread to indicate that the queue was fully
    * filled with requests, and no more elements will be added.
    */
   bool filled;

   /** 
    * Condition Variable that is used to signal all threads when 
    * an element has been added, removed, or the filled flag has been updated.
    */
   pthread_cond_t *updatedCondition;
} ThreadSafeQueue;

ThreadSafeQueue* createTSQueue();

/**
 * Adds an element to the end queue.
 * This function is thread safe, as it will use the Queue's internal
 * mutex to ensure it has sole access to the queue while adding the element.
 * 
 * @param element A new LinkedQueueElement. If dynamically allocated it is the job of
 *                the consumer which calls TSQueue_pop to free it.
 */
void TSQueue_add(ThreadSafeQueue *queue, LinkedQueueElement *element);

/**
 * Pops an element from the head of the queue.
 * This function is thread safe, as it will use the Queue's internal
 * mutex to ensure it has sole access to the queue while removing the element.
 *
 * @retval Pointer to the popped element, or NULL if the queue is empty.
 */
LinkedQueueElement* TSQueue_pop(ThreadSafeQueue *queue);

/**
 * Pops an element from the head of the queue.
 * This is the thread-UNSAFE version of TSQueue_pop,
 * requiring manual manipulation of the queue's lock.
 *
 * Ensure the queue's mutex is LOCKED before calling this function!
 * Unlock the mutex after this function completes.
 *
 * @retval Pointer to the popped element, or NULL if the queue is empty.
 */
LinkedQueueElement* TSQueue_popUnsafe(ThreadSafeQueue *queue);

/**
 * Cleans up the queue.
 *
 * If any elements are still present in the queue it will free() them.
 */
void TSQueue_cleanup(ThreadSafeQueue **queue);

#endif // _QUEUE_H
