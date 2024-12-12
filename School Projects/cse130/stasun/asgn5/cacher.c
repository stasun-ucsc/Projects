#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include "cacher.h"
#include "list.h"

#define OPTIONS "N:FLC"

typedef struct CacheObj {
    List cache;
    List history;
    int size;
    int comp_miss;
    int cap_miss;
    int clock_index;
} CacheObj;

Cache newCache(int size) {
    Cache cache = malloc(sizeof(CacheObj));
    cache->cache = newList();
    cache->history = newList();
    cache->size = size;
    cache->comp_miss = 0;
    cache->cap_miss = 0;
    cache->clock_index = 0;

    return cache;
}

void freeCache(Cache *pC) {
    if (pC != NULL && *pC != NULL) {
        freeList(&((*pC)->cache));
        freeList(&((*pC)->history));
    }

    free(*pC);
    *pC = NULL;

    return;
}

// FIFO
int insert_fifo(Cache c, char *elem) {
    if (c == NULL) {
        // exit with an error
        fprintf(stderr, "Cache Error: Calling insert_fifo() on NULL Cache reference.\n");
        return EXIT_FAILURE;
    }

    if (elem == NULL) {
        // exit with an error
        fprintf(stderr, "Cache Error: Calling insert_fifo() on NULL elem reference.\n");
        return EXIT_FAILURE;
    }

    // Check if the list contains the element
    if (search(c->cache, elem)) {
        return 1; // Returns true if the element is in the cache
    }

    // Check if the list is full
    if (length(c->cache) >= c->size) {
        deleteFront(c->cache);

        // Check if history list contains the element
        if (!search(c->history, elem)) {
            // missing history list update?
            append(c->history, elem);
            c->comp_miss++;
        } else {
            c->cap_miss++;
        }
    }

    else {
        // check if history list contains the element
        // i don't this this is necessary

        /* if (!search(c -> history, elem)) {
                        c -> comp_miss++;
                } */

        c->comp_miss++;
    }

    // Add the element to the back of the list
    append(c->cache, elem);
    // missing history list update?
    append(c->history, elem);

    return 0; // return false if the element was not in the cache
}

// LRU
int insert_lru(Cache c, char *elem) {
    if (c == NULL) {
        // exit with an error
        fprintf(stderr, "Cache Error: Calling insert_lru() on NULL Cache reference.\n");
        return EXIT_FAILURE;
    }

    if (elem == NULL) {
        // exit with an error
        fprintf(stderr, "Cache Error: Calling insert_lru() on NULL elem reference.\n");
        return EXIT_FAILURE;
    }

    // Check if the list contains the element
    if (search(c->cache, elem)) {
        // Move the element to the back of the list
        append(c->cache, get(c->cache));
        delete (c->cache);

        return 1;
    }

    // Check if the list is full
    if (length(c->cache) >= c->size) {
        deleteFront(c->cache);

        // Check if history list contains the element
        if (!search(c->history, elem)) {
            // missing history list update?
            append(c->history, elem);
            c->comp_miss++;
        } else {
            c->cap_miss++;
        }

    }

    else {
        c->comp_miss++;
    }
    // Add the element to the back of the list
    append(c->cache, elem);
    // missing history list update?
    append(c->history, elem);

    return 0; //Return false if the element was not in the cache
}

// Clock
int insert_clock(Cache c, char *elem) {
    if (c == NULL) {
        // exit with an error
        fprintf(stderr, "Cache Error: Calling insert_clock() on NULL Cache reference.\n");
        return EXIT_FAILURE;
    }

    if (elem == NULL) {
        // exit with an error
        fprintf(stderr, "Cache Error: Calling insert_clock() on NULL elem reference.\n");
        return EXIT_FAILURE;
    }

    // check if cache has element
    Node node;
    if ((node = nodeSearch(c->cache, elem)) != NULL) {
        node->clock_bit = 1;
        return 1;
    }

    // check if cache has space
    if (length(c->cache) >= c->size) {
        bool evict = false;
        while (!evict) {
            if (getIndex(c->cache) == -1 && length(c->cache) > 0) {
                moveFront(c->cache);
            }

            if (getCursor(c->cache)->clock_bit == 1) {
                getCursor(c->cache)->clock_bit = 0;
                moveNext(c->cache);
            }

            else {
                insertAfter(c->cache, elem);

                int index = getIndex(c->cache);
                delete (c->cache);
                moveFront(c->cache);
                while (getIndex(c->cache) != index) {
                    moveNext(c->cache);
                }
                moveNext(c->cache);
                evict = true;
            }
        }
        // check if elem is in history
        if (search(c->history, elem)) {
            c->cap_miss++;
        } else {
            c->comp_miss++;
            append(c->history, elem);
        }
    } else {
        append(c->cache, elem);
        c->comp_miss++;
        append(c->history, elem);

        return 0;
    }

    return 0;
}

int main(int argc, char **argv) {
    if (argc < 3) {
        fprintf(stderr, "insufficient arguments\n");
        return EXIT_FAILURE;
    }
    int opt = 0;
    int size;
    bool N_flag = false, F_flag = false, L_flag = false, C_flag = false;
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'N':
            // printf("N\n");
            size = atoi(optarg);
            if (size <= 0) {
                fprintf(stderr, "invalid size\n");
                return EXIT_FAILURE;
            }

            N_flag = true;
            break;
        case 'F':
            // printf("F\n");
            if (C_flag || L_flag) {
                fprintf(stderr, "too many flags\n");
                return EXIT_FAILURE;
            }

            F_flag = true;
            break;
        case 'L':
            // printf("L\n");
            if (F_flag || C_flag) {
                fprintf(stderr, "too many flags\n");
                return EXIT_FAILURE;
            }

            L_flag = true;
            break;
        case 'C':
            // printf("C\n");
            if (F_flag || L_flag) {
                fprintf(stderr, "too many flags\n");
                return EXIT_FAILURE;
            }

            C_flag = true;
            break;
        default: fprintf(stderr, "Error: switch default case\n"); exit(EXIT_FAILURE);
        }
    }

    if (!N_flag) {
        fprintf(stderr, "No cache size specified\n");
        return EXIT_FAILURE;
    }

    if (!F_flag && !L_flag && !C_flag) {
        F_flag = true;
    }

    char entry[4096];
    Cache cache = newCache(size);

    // this implementation might not pick up empty strings...
    while (fscanf(stdin, "%s\n", entry) >= 0) {
        if (F_flag) {
            if (insert_fifo(cache, entry)) {
                fprintf(stdout, "HIT\n");
            } else {
                fprintf(stdout, "MISS\n");
            }
        }

        else if (L_flag) {
            if (insert_lru(cache, entry)) {
                fprintf(stdout, "HIT\n");
            } else {
                fprintf(stdout, "MISS\n");
            }
        }

        else if (C_flag) {
            if (insert_clock(cache, entry)) {
                fprintf(stdout, "HIT\n");
            } else {
                fprintf(stdout, "MISS\n");
            }
        }

        else {
            fprintf(stderr, "Invalid policy\n");
            exit(EXIT_FAILURE);
        }
    }

    fprintf(stdout, "%d %d\n", cache->comp_miss, cache->cap_miss);
    // printList(stdout, cache -> cache);
    // printList(stdout, cache -> history);

    freeCache(&cache);

    return 0;
}
