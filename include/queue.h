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
    * Condition Variable that is used to signal all threads when 
    * an element has been added to the queue
    */
   pthread_cond_t *elementAddedCondition;
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
 * Cleans up the queue.
 *
 * If any elements are still present in the queue it will free() them.
 */
void TSQueue_cleanup(ThreadSafeQueue **queue);

#endif // _QUEUE_H
