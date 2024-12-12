// Adapted from Patrick Tantalo's CSE101 PA1 in Spring 2023.

//-----------------------------------------------------------------------------
// List.c
// Implementation file for List ADT
//-----------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <pthread.h>

#include "list.h"
#include "rwlock.h"

// structs --------------------------------------------------------------------

// private Node type
typedef struct NodeObj *Node;

// private NodeObj type
typedef struct NodeObj {
    char uri[64];
    rwlock_t *lock;
    Node prev;
    Node next;
} NodeObj;

// private ListObj type
typedef struct ListObj {
    Node front;
    Node back;
    Node cursor;
    int cursorIndex;
    int length;
    pthread_mutex_t listLock;
} ListObj;

// prototypes -----------------------------------------------------------------

Node getCursor(List L);
Node getFront(List L);
Node getBack(List L);

// Constructors-Destructors ---------------------------------------------------

// implement the list != NULL precondition

// newNode()
// Returns reference to new Node object. Initializes next, prev, and data fields.
// Adapted for this assignment, ChatGPT used for assistance with threadsafe
// implementation
Node newNode(const char *uri) {
    Node N = malloc(sizeof(NodeObj));
    /* assert(N != NULL); */
    strncpy(N->uri, uri, 64);
    N->lock = rwlock_new(N_WAY, 1);

    N->prev = NULL;
    N->next = NULL;

    return (N);
}

// freeNode()
// Frees heap memory pointed to by *pN, sets *pN to NULL.
/* void freeNode(Node* pN){
   	if( pN != NULL && *pN != NULL ) {
      		free(*pN);
      		*pN = NULL;
   	}

	return;
} */

// newList()
// Creates and returns a new empty List.
List newList(void) {
    List list = malloc(sizeof(ListObj));
    /* assert(list != NULL); */
    list->front = NULL;
    list->back = NULL;
    list->cursor = NULL;
    list->cursorIndex = -1;
    list->length = 0;
    pthread_mutex_init(&(list->listLock), NULL);

    return (list);
}

// freeList()
// Frees all heap memory associated with *pL, and sets *pL to NULL
/* void freeList(List* pL) {
	if (pL != NULL && *pL != NULL) { 
		while (length(*pL) != 0) { 
			clear(*pL); 
      		}	
      	
	free(*pL);
      	*pL = NULL;
   	}	

	return;
} */

// prepend()
// Insert new element into L. If L is non-empty,
// insertion takes place before front element.
// Returns pointer to newly created lock.
rwlock_t *prepend(List L, char *uri) {
    if (L == NULL) {
        printf("List Error: calling prepend() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }

    Node new = newNode(uri);
    if (length(L) > 0) {
        getFront(L)->prev = new;
        new->next = getFront(L);
    }

    L->front = new;
    if (length(L) == 0) {
        L->back = new;
    }

    L->length++; /* see if you can use length() */
    if (cur_index(L) != -1) {
        L->cursorIndex++;
    }

    return new->lock;
}

// search()
// searches for the node containing a specific URI. If found, it will return the
// address of the rwlock. Otherwise it will append a new node with the URI and
// return the newly created lock.

rwlock_t *search(List L, char *uri) {
    pthread_mutex_lock(&(L->listLock));

    if (L == NULL) {
        printf("List Error: calling search() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }

    rwlock_t *lock;
    moveFront(L);

    while (cur_index(L) != -1) {
        if (strncmp(uri, getCursor(L)->uri, 64) == 0) {
            lock = get(L);
            pthread_mutex_unlock(&(L->listLock));
            return lock;
        }

        moveNext(L);
    }

    lock = prepend(L, uri);
    pthread_mutex_unlock(&(L->listLock));

    return lock;
}

// get()
// Returns the rwlock stored at the cursor of L. Pre: length()>0, index()>=0
rwlock_t *get(List L) {
    if (L == NULL) {
        printf("List Error: calling get() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }

    if (length(L) <= 0) {
        printf("List Error: calling get() on an empty List\n");
        exit(EXIT_FAILURE);
    }

    if (cur_index(L) < 0) {
        printf("List Error: calling get() on a List with undefined cursor\n");
        exit(EXIT_FAILURE);
    }

    return getCursor(L)->lock;
}

// getCursor()
// Returns address of cursor element in L
Node getCursor(List L) {
    if (L == NULL) {
        printf("List Error: calling getCursor() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }

    return L->cursor;
}

// moveFront()
// If L is non-empty, sets cursor under the front element,
// otherwise does nothing.
void moveFront(List L) {
    if (L == NULL) {
        printf("List Error: calling moveFront() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }

    if (length(L) > 0) {
        L->cursorIndex = 0;
        L->cursor = getFront(L);
    }

    return;
}

// getFront()
// Returns address of front element in L
Node getFront(List L) {
    if (L == NULL) {
        printf("List Error: calling getFront() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }

    return L->front;
}

// moveNext()
// If cursor is defined and not at back, move cursor one
// step toward the back of L; if cursor is defined and at
// back, cursor becomes undefined; if cursor is undefined
// do nothing
void moveNext(List L) {
    if (L == NULL) {
        printf("List Error: calling moveNext() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    if (cur_index(L) > -1) {
        L->cursorIndex++;
        L->cursor = getCursor(L)->next;
    }

    if (cur_index(L) >= length(L)) {
        L->cursorIndex = -1;
        L->cursor = NULL;
    }

    return;
}

// length()
// Returns the number of elements in L.
int length(List L) {
    if (L == NULL) {
        printf("List Error: calling length() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }

    return L->length;
}

// cur_index()
// Returns index of cursor element if defined, -1 otherwise.
int cur_index(List L) {
    if (L == NULL) {
        printf("List Error: calling index() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }

    return L->cursorIndex;
}
