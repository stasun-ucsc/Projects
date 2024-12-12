#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <errno.h>
#include "queue.h"

struct queue {
    void **array;
    int max_size;
    int len;
    int front;
    int back;
    sem_t empty;
    sem_t full;
    // sem_t mutex;
    sem_t push_mutex;
    sem_t pop_mutex;
};

// typedef struct queue queue_t;

queue_t *queue_new(int size) {
    if (size < 0) {
        size = 0;
    }

    queue_t *pNew = malloc(sizeof(queue_t));
    pNew->array = calloc(size, sizeof(void *));
    pNew->max_size = size;
    pNew->len = 0;
    pNew->front = 0;
    pNew->back = 0;

    sem_init(&(pNew->empty), 0, 0);
    // third arg needs to be unsigned int, could cause problem
    sem_init(&(pNew->full), 0, size);
    // sem_init(&(pNew->mutex), 0, 1);
    sem_init(&(pNew->push_mutex), 0, 1);
    sem_init(&(pNew->pop_mutex), 0, 1);

    return pNew;
}

void queue_delete(queue_t **q) {
    if (q != NULL) {
        if (*q != NULL) {
            for (int i = 0; i < (*q)->len; i++) {
                // do some modulo stuff for the iteration
                if ((*q)->array[(i + (*q)->front) % (*q)->max_size] != NULL) {
                    free((*q)->array[(i + (*q)->front) % (*q)->max_size]);
                    (*q)->array[(i + (*q)->front) % (*q)->max_size] = NULL;
                }
            }

            free((*q)->array);
            (*q)->array = NULL;

            sem_destroy(&((*q)->empty));
            sem_destroy(&((*q)->full));
            // sem_destroy(&((*q)->mutex));
            sem_destroy(&((*q)->push_mutex));
            sem_destroy(&((*q)->pop_mutex));

            free(*q);
            *q = NULL;
        }
    }

    return;
}

bool queue_push(queue_t *q, void *elem) {
    if (q == NULL) {
        return false;
    }

    /* if (q -> len == q -> max_size) {
		printf("full queue\n");
		return false;
	} */

    sem_wait(&(q->full));
    sem_wait(&(q->push_mutex));

    q->array[q->back] = elem;
    q->back = (q->back + 1) % q->max_size;
    q->len++;

    sem_post(&(q->push_mutex));
    sem_post(&(q->empty));

    return true;
}

bool queue_pop(queue_t *q, void **elem) {
    if (q == NULL || elem == NULL) {
        return false;
    }

    /* if (q -> len == 0) {
		printf("empty queue\n");
		return false;
	} */

    sem_wait(&(q->empty));
    sem_wait(&(q->pop_mutex));

    *elem = (q->array)[q->front];
    q->array[q->front] = NULL;
    q->len--;
    q->front = (q->front + 1) % q->max_size;

    sem_post(&(q->pop_mutex));
    sem_post(&(q->full));

    return true;
}
