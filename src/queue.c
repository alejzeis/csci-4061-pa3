#include "queue.h"

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

ThreadSafeQueue* createTSQueue()
{
    ThreadSafeQueue *queue = malloc(sizeof(ThreadSafeQueue));

    queue->lock = malloc(sizeof(pthread_mutex_t));
    queue->elementAddedCondition = malloc(sizeof(pthread_cond_t));
    pthread_mutex_init(queue->lock, NULL);
    pthread_cond_init(queue->elementAddedCondition, NULL);

    queue->head = NULL;
    queue->tail = NULL;

    return queue;
}

void TSQueue_add(ThreadSafeQueue *queue, LinkedQueueElement *element)
{
    element->next = NULL;
    element->previous = NULL;

    // Lock the mutex so that only we are modifying the queue
    pthread_mutex_lock(queue->lock);

    if (queue->head == NULL)
    {
        // Queue is empty if this is true

        queue->head = element;
        queue->tail = element;
    }
    else
    {
        // Add to end of queue
        element->next = queue->tail;
        queue->tail->previous = element;
        queue->tail = element;
    }

    // Notify any threads listening that a new element has been added
    pthread_cond_broadcast(queue->elementAddedCondition);

    pthread_mutex_unlock(queue->lock);
}

LinkedQueueElement* TSQueue_pop(ThreadSafeQueue *queue)
{
    LinkedQueueElement *poppedElement;

    // Lock the mutex so that only we are modifying the queue
    pthread_mutex_lock(queue->lock);

    if (queue->head == NULL)
    {
       poppedElement = NULL;
    }
    else
    {
       poppedElement = queue->head;

       queue->head = poppedElement->previous;

       poppedElement->next = NULL;
       poppedElement->previous = NULL;
    }

    pthread_mutex_unlock(queue->lock);

    return poppedElement;
}

void TSQueue_cleanup(ThreadSafeQueue **queue)
{
    LinkedQueueElement *e;
    // Free any remaining elements
    while ((e = TSQueue_pop(*queue)) != NULL)
    {
        free(e->file); // Dynamically allocated in processing()
        free(e);
    }

    free((*queue)->lock);
    free((*queue)->elementAddedCondition);
    free(*queue);

    *queue = NULL;
}
