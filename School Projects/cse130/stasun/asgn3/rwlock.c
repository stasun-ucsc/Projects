#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <semaphore.h>
#include <pthread.h>
#include <stdbool.h>
#include "rwlock.h"

// General structure of the code and helper functions and pseudocode from Mitchell's section on 11/7

int reader_wait(rwlock_t *rw);
int writer_wait(rwlock_t *rw);
void rw_wakeup(rwlock_t *rw);

struct rwlock {
    PRIORITY priority;
    uint32_t n;
    pthread_mutex_t mutex;
    pthread_cond_t read_cv;
    pthread_cond_t write_cv;
    int active_readers;
    int active_writers;
    int waiting_readers;
    int waiting_writers;
    int n_count;
    bool flag;
};

rwlock_t *rwlock_new(PRIORITY p, uint32_t n) {
    rwlock_t *pNew = malloc(sizeof(rwlock_t));
    pNew->priority = p;
    pNew->n = n;

    pthread_mutex_init(&(pNew->mutex), NULL);
    pthread_cond_init(&(pNew->read_cv), NULL);
    pthread_cond_init(&(pNew->write_cv), NULL);

    pNew->active_readers = 0;
    pNew->active_writers = 0;
    pNew->waiting_readers = 0;
    pNew->waiting_writers = 0;
    pNew->n_count = 0;
    pNew->flag = false;

    return pNew;
}

void rwlock_delete(rwlock_t **rw) {
    if (rw != NULL) {
        if (*rw != NULL) {
            pthread_mutex_destroy(&((*rw)->mutex));
            pthread_cond_destroy(&((*rw)->read_cv));
            pthread_cond_destroy(&((*rw)->write_cv));
        }
    }

    free(*rw);
    *rw = NULL;

    return;
}

void reader_lock(rwlock_t *rw) {
    // lock the mutex
    pthread_mutex_lock(&(rw->mutex));
    // increment the waiting reader count
    rw->waiting_readers++;

    while (reader_wait(rw)) {
        // make the thread wait
        // not sure if this is correct
        pthread_cond_wait(&(rw->read_cv), &(rw->mutex));
    }

    // decrement the waiting reader count
    rw->waiting_readers--;
    // increment the active reader count
    rw->active_readers++;

    if (rw->priority == N_WAY) {
        if (rw->flag && rw->n_count == 0) {
            // unset the flag
            rw->flag = false;
        }

        if (!(rw->flag) && (rw->n_count) < (int) (rw->n)) {
            // increment nwaycount
            rw->n_count++;
        }
    }

    // unlock the mutex
    pthread_mutex_unlock(&(rw->mutex));

    return;
}

void reader_unlock(rwlock_t *rw) {
    // lock the mutex
    pthread_mutex_lock(&(rw->mutex));
    // decrement the active reader count
    rw->active_readers--;

    if (rw->priority == N_WAY) {
        if (rw->n_count == (int) rw->n) {
            // set the flag
            rw->flag = true;
        }

        if (rw->flag == true && rw->n_count > 0) {
            // reset the nwaycount to 0
            rw->n_count = 0;
        }
    }

    // wake up all of the threads
    // call pthread_cond_broadcast on the reader and writer cvs
    rw_wakeup(rw);
    // unlock the mutex
    pthread_mutex_unlock(&(rw->mutex));

    return;
}

void writer_lock(rwlock_t *rw) {
    // lock the mutex
    pthread_mutex_lock(&(rw->mutex));

    // increment the waiting writer count
    rw->waiting_writers++;

    while (writer_wait(rw)) {
        // make the thread wait
        pthread_cond_wait(&(rw->write_cv), &(rw->mutex));
    }

    // decrement the waiting writer count
    rw->waiting_writers--;
    // increment the active writer count
    rw->active_writers++;

    // unlock the mutex
    pthread_mutex_unlock(&(rw->mutex));

    return;
}

void writer_unlock(rwlock_t *rw) {
    // lock the mutex
    pthread_mutex_lock(&(rw->mutex));

    // decrement the active writer count
    rw->active_writers--;

    if (rw->priority == N_WAY) {
        if (rw->flag) {
            // unset the flag
            rw->flag = false;
        }
    }

    // wake up all of the threads
    // broadcast to reader and writer cvs
    rw_wakeup(rw);

    // unlock the mutex
    pthread_mutex_unlock(&(rw->mutex));

    return;
}

int reader_wait(rwlock_t *rw) {
    if (rw->priority == READERS) {
        return rw->active_writers;
    }

    else if (rw->priority == WRITERS) {
        return (rw->active_writers || rw->waiting_writers);
    }

    else if (rw->priority == N_WAY) {
        if (rw->active_readers) {
            if (rw->waiting_writers) {
                if ((rw->n_count) < (int) (rw->n)) {
                    return 0;
                }

                else {
                    if (rw->flag) {
                        return rw->waiting_writers;
                    } else {
                        return 0;
                    }
                }
            } else {
                return 0;
            }
        } else if (rw->active_writers) {
            return rw->active_writers;
        }
        // no active readers or writers
        else if (rw->active_readers == 0 || rw->active_writers == 0) {
            if (rw->flag) {
                return (rw->waiting_writers);
            } else {
                return 0;
            }
        }

    }

    else {
        // throw an error
        exit(EXIT_FAILURE);
    }

    exit(EXIT_FAILURE);
}

int writer_wait(rwlock_t *rw) {
    if (rw->priority == READERS) {
        return (rw->active_writers || rw->active_readers);
    }

    else if (rw->priority == WRITERS) {
        return (rw->active_writers || rw->active_readers);
    }

    else if (rw->priority == N_WAY) {
        if (rw->active_readers) {
            return rw->active_readers;
        } else if (rw->active_writers) {
            return rw->active_writers;
        }
        // no active readers or writers
        else {
            if (!(rw->flag)) {
                return rw->waiting_readers;
            } else {
                return 0;
            }
        }
    }

    else {
        // throw a party!!! jk throw an error
        exit(EXIT_FAILURE);
    }
}

void rw_wakeup(rwlock_t *rw) {
    if (rw->waiting_readers) {
        // broadcast to all of the readers
        pthread_cond_broadcast(&(rw->read_cv));
    }

    if (rw->waiting_writers) {
        // broadcast to all of the writers
        pthread_cond_broadcast(&(rw->write_cv));
    }

    return;
}
