//-----------------------------------------------------------------------------
// list.c
// Implementation file for List ADT
//-----------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "list.h"

// structs --------------------------------------------------------------------

// private Node type
// typedef struct NodeObj* Node;

// private NodeObj type
/* typedef struct NodeObj{
   	char data[4096];
	int clock_bit;
   	Node prev;
	Node next;
} NodeObj; */

// private ListObj type
/* typedef struct ListObj{
   	Node front;
   	Node back;
	Node cursor;
   	int cursorIndex;
   	int length;
} ListObj; */

// prototypes -----------------------------------------------------------------

/* Node getCursor(List L);
Node getFront(List L);
Node getBack(List L);
Node nodeSearch(List L, char *elem);*/ // new
/* void listTracker(List L) {
	printf("front = %p\n", getFront(L));
        printf("back = %p\n", getBack(L));
        printf("cursor = %p\n", getCursor(L));
        printf("index = %d\n", getIndex(L));
        printf("length = %d\n\n", length(L));
	return;
} */

/* int main(void) {
        List L = newList();
	moveFront(L);
        listTracker(L);
	
	append(L, 7);
	listTracker(L);
	printList(stdout, L);
	
	deleteBack(L);
	listTracker(L);

	append(L, 69);
	listTracker(L);
	printList(stdout, L);
	
	prepend(L, 420);
	listTracker(L);
	printList(stdout, L);
	
	moveFront(L);
	listTracker(L);
	movePrev(L); 
	listTracker(L);
	
	moveBack(L);
        listTracker(L);
        moveNext(L);
        listTracker(L);

	moveFront(L);
	listTracker(L);
	moveNext(L);
	listTracker(L);

	clear(L);
	listTracker(L);

	deleteFront(L);
	printList(stdout, L);
	listTracker(L);

	freeList(&L);
        return 0;
} */

// Constructors-Destructors ---------------------------------------------------

// implement the list != NULL precondition

// newNode()
// Returns reference to new Node object. Initializes next, prev, and data fields.
Node newNode(char *data) {
    Node N = malloc(sizeof(NodeObj));
    /* assert(N != NULL); */
    if (strlen(data) > 4096) {
        fprintf(stderr, "Length of input string to long (max 4096 chars)\n");
        exit(EXIT_FAILURE);
    }

    strncpy(N->data, data, 4096);
    N->clock_bit = 0;
    N->prev = NULL;
    N->next = NULL;

    return (N);
}

// freeNode()
// Frees heap memory pointed to by *pN, sets *pN to NULL.
void freeNode(Node *pN) {
    if (pN != NULL && *pN != NULL) {
        free(*pN);
        *pN = NULL;
    }

    return;
}

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

    return (list);
}

// freeList()
// Frees all heap memory associated with *pL, and sets *pL to NULL
void freeList(List *pL) {
    if (pL != NULL && *pL != NULL) {
        while (length(*pL) != 0) {
            clear(*pL);
        }

        free(*pL);
        *pL = NULL;
    }

    return;
}

// New functions --------------------------------------------------------------
int search(List L, char *elem) {
    if (L == NULL) {
        printf("List Error: calling search() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }

    if (elem == NULL) {
        printf("List Error: calling search() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }

    if (length(L) == 0) {
        return 0;
    }

    moveFront(L);
    do {
        if (!strncmp(elem, get(L), 4096)) {
            return 1;
        }

        moveNext(L);
    } while (getIndex(L) != -1);

    return 0;
}

Node nodeSearch(List L, char *elem) {
    if (L == NULL) {
        printf("List Error: calling search() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }

    if (elem == NULL) {
        printf("List Error: calling search() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }

    if (length(L) == 0) {
        return 0;
    }

    Node curr = getFront(L);
    do {
        if (!strncmp(elem, curr->data, 4096)) {
            return curr;
        }

        curr = curr->next;
    } while (curr != NULL);

    return NULL;
}

// Access functions -----------------------------------------------------------

// length()
// Returns the number of elements in L.
int length(List L) {
    if (L == NULL) {
        printf("List Error: calling length() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }

    return L->length;
}

// getIndex()
// Returns index of cursor element if defined, -1 otherwise.
int getIndex(List L) {
    if (L == NULL) {
        printf("List Error: calling getIndex() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }

    return L->cursorIndex;
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

// getFront()
// Returns address of front element in L
Node getFront(List L) {
    if (L == NULL) {
        printf("List Error: calling getFront() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }

    return L->front;
}

// getBack()
// Returns address of back element in L
Node getBack(List L) {
    if (L == NULL) {
        printf("List Error: calling getBack() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }

    return L->back;
}

// front()
// Returns front element of L. Pre: length()>0
char *front(List L) {
    if (L == NULL) {
        printf("List Error: calling front() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }

    if (length(L) <= 0) {
        printf("List Error: calling front() on an empty List\n");
        exit(EXIT_FAILURE);
    }

    return getFront(L)->data;
}

// back()
// Returns back element of L. Pre: length()>0
char *back(List L) {
    if (L == NULL) {
        printf("List Error: calling back() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }

    if (length(L) <= 0) {
        printf("List Error: calling back() on an empty List\n");
        exit(EXIT_FAILURE);
    }

    return getBack(L)->data;
}

// get()
// Returns cursor element of L. Pre: length()>0, getIndex()>=0
char *get(List L) {
    if (L == NULL) {
        printf("List Error: calling get() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }

    if (length(L) <= 0) {
        printf("List Error: calling get() on an empty List\n");
        exit(EXIT_FAILURE);
    }

    if (getIndex(L) < 0) {
        printf("List Error: calling get() on a List with undefined cursor\n");
        exit(EXIT_FAILURE);
    }

    return getCursor(L)->data;
}

// equals()
// Returns true if Lists A and B are in same
// state, and returns false otherwise.
bool equals(List A, List B) {
    if (A == NULL || B == NULL) {
        printf("List Error: calling equals() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }

    if (length(A) != length(B)) {
        return false;
    }

    else {
        moveFront(A);
        moveFront(B);
        while (getIndex(A) != -1) {
            if (get(A) != get(B)) {
                return false;
            }

            else {
                moveNext(A);
                moveNext(B);
            }
        }
        /* Node currA = getFront(A);
		Node currB = getFront(B);
		for (int i = 0; i < length(A); i++) {
			if (currA -> data != currB -> data) {
				return false;
			}
			
			else {
				currA = currA -> next;
				currB = currB -> next;
			}	
		} */

        return true;
    }
}

// Manipulation procedures ----------------------------------------------------

// clear()
// Resets L to its original empty state.
void clear(List L) {
    if (L == NULL) {
        printf("List Error: calling clear() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }

    while (length(L) > 0) {
        deleteBack(L);
        /* L -> length--;		 idk might cause problems */
    }

    L->cursorIndex = -1;
    L->cursor = NULL;
    return;
}

// set()
// Overwrites the cursor element’s data with x.
// Pre: length()>0, getIndex()>=0
void set(List L, char *x) {
    if (L == NULL) {
        printf("List Error: calling set() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }

    if (length(L) <= 0) {
        printf("List Error: calling set() on an empty List\n");
        exit(EXIT_FAILURE);
    }

    if (getIndex(L) < 0) {
        printf("List Error: calling set() with an undefined Cursor\n");
        exit(EXIT_FAILURE);
    }

    strncpy(getCursor(L)->data, x, 4096);
    return;
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

// moveBack()
// If L is non-empty, sets cursor under the back element,
// otherwise does nothing.
void moveBack(List L) {
    if (L == NULL) {
        printf("List Error: calling moveBack() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }

    if (length(L) > 0) {
        L->cursorIndex = length(L) - 1;
        L->cursor = getBack(L);
    }

    return;
}

// movePrev()
// If cursor is defined and not at front, move cursor one
// step toward the front of L; if cursor is defined and at
// front, cursor becomes undefined; if cursor is undefined
// do nothing
void movePrev(List L) {
    if (L == NULL) {
        printf("List Error: calling movePrev() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }

    if (getIndex(L) > -1) {
        L->cursorIndex--;
        L->cursor = getCursor(L)->prev;
    }

    return;
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
    if (getIndex(L) > -1) {
        L->cursorIndex++;
        L->cursor = getCursor(L)->next;
    }

    if (getIndex(L) >= length(L)) {
        L->cursorIndex = -1;
        L->cursor = NULL;
    }

    return;
}

// prepend()
// Insert new element into L. If L is non-empty,
// insertion takes place before front element.
void prepend(List L, char *x) {
    if (L == NULL) {
        printf("List Error: calling prepend() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }

    Node new = newNode(x);
    if (length(L) > 0) {
        getFront(L)->prev = new;
        new->next = getFront(L);
    }

    L->front = new;
    if (length(L) == 0) {
        L->back = new;
    }

    L->length++; /* see if you can use length() */
    if (getIndex(L) != -1) {
        L->cursorIndex++;
    }

    return;
}

// append()
// Insert new element into L. If L is non-empty,
// insertion takes place after back element.
void append(List L, char *x) {
    if (L == NULL) {
        printf("List Error: calling append() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }

    Node new = newNode(x);
    if (length(L) > 0) {
        getBack(L)->next = new;
        new->prev = getBack(L);
    }

    L->back = new;
    if (length(L) == 0) {
        L->front = new;
    }

    L->length += 1;
    return;
}

// insertBefore()
// Insert new element before cursor.
// Pre: length()>0, getIndex()>=0
void insertBefore(List L, char *x) {
    if (L == NULL) {
        printf("List Error: calling insertBefore() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }

    if (length(L) <= 0) {
        printf("List Error: calling insertBefore() on an empty List\n");
        exit(EXIT_FAILURE);
    }

    if (getIndex(L) < 0) {
        printf("List Error: calling insertBefore() on a List with undefined cursor\n");
        exit(EXIT_FAILURE);
    }

    if (getIndex(L) == 0) {
        prepend(L, x);
        return;
    }

    Node cursorNode = getCursor(L);
    Node new = newNode(x);

    cursorNode->prev->next = new;
    new->prev = cursorNode->prev;
    new->next = cursorNode;
    cursorNode->prev = new;

    L->length++;
    L->cursorIndex++;
    return;
}

// insertAfter()
// Insert new element after cursor.
// Pre: length()>0, getIndex()>=0
void insertAfter(List L, char *x) {
    if (L == NULL) {
        printf("List Error: calling insertAfter() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }

    if (length(L) <= 0) {
        printf("List Error: calling insertAfter() on an empty List\n");
        exit(EXIT_FAILURE);
    }

    if (getIndex(L) < 0) {
        printf("List Error: calling insertAfter() on a List with undefined cursor\n");
        exit(EXIT_FAILURE);
    }

    if (getIndex(L) == length(L) - 1) {
        append(L, x);
        return;
    }

    Node cursorNode = getCursor(L);
    Node new = newNode(x);

    cursorNode->next->prev = new;
    new->next = cursorNode->next;
    new->prev = cursorNode;
    cursorNode->next = new;

    L->length++;
    return;
}

// deleteFront()
// Delete the front element. Pre: length()>0
void deleteFront(List L) {
    if (L == NULL) {
        printf("List Error: calling deleteFront() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }

    if (length(L) <= 0) {
        printf("List Error: calling deleteFront() on an empty List\n");
        exit(EXIT_FAILURE);
    }

    if (getIndex(L) == 0) {
        L->cursor = NULL;
    }

    L->front = getFront(L)->next;
    if (getFront(L) != NULL) {
        freeNode(&(getFront(L)->prev));
        getFront(L)->prev = NULL;
    }

    else {
        Node temp = getBack(L);
        freeNode(&temp);
    }

    if (getIndex(L) != -1) {
        L->cursorIndex--;
    }

    L->length--;
    if (length(L) == 0) {
        L->back = NULL;
    }

    return;
}

// deleteBack()
// Delete the back element. Pre: length()>0
void deleteBack(List L) {
    if (L == NULL) {
        printf("List Error: calling deleteBack() on NULL List reference\n");

        exit(EXIT_FAILURE);
    }

    if (length(L) <= 0) {
        printf("List Error: calling deleteBack() on an empty List\n");
        exit(EXIT_FAILURE);
    }

    if (getIndex(L) == length(L) - 1) {
        L->cursorIndex = -1;
        L->cursor = NULL;
    }

    L->back = getBack(L)->prev;
    if (getBack(L) != NULL) {
        freeNode(&(getBack(L)->next));
        getBack(L)->next = NULL;
    }

    else {
        Node temp = getFront(L);
        freeNode(&temp);
    }

    L->length--;
    if (length(L) == 0) {
        L->front = NULL;
    }

    return;
}

// delete()
// Delete cursor element, making cursor undefined.
// Pre: length()>0, getIndex()>=0
void delete (List L) {
    if (L == NULL) {
        printf("List Error: calling delete() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }

    if (length(L) <= 0) {
        printf("List Error: calling delete() on an empty List\n");
        exit(EXIT_FAILURE);
    }

    if (getIndex(L) < 0) {
        printf("List Error: calling delete() on a List with undefined cursor\n");
        exit(EXIT_FAILURE);
    }

    if (getIndex(L) == 0) {
        deleteFront(L);
        return;
    }

    if (getIndex(L) == length(L) - 1) {
        deleteBack(L);
        return;
    }

    Node cursorNode = getCursor(L); /* this is code is deffo missing stuff */

    cursorNode->prev->next = cursorNode->next;
    cursorNode->next->prev = cursorNode->prev;

    freeNode(&cursorNode);
    cursorNode = NULL;

    L->length--;
    L->cursorIndex = -1;
    L->cursor = NULL;

    return;
}

// Other operations -----------------------------------------------------------

// printList()
// Prints to the file pointed to by out, a
// string representation of L consisting
// of a space separated sequence of integers,
// with front on left.
void printList(FILE *out, List L) {
    if (L == NULL) {
        printf("List Error: calling printList() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }

    if (out == NULL) {
        printf("List Error: calling printList() on NULL FILE pointer reference\n");
        exit(EXIT_FAILURE);
    }

    Node currNode = getFront(L);
    for (int i = 0; i < length(L); i++) {
        if (i == length(L) - 1) {
            fprintf(out, "%s\n", currNode->data);
        }

        else {
            fprintf(out, "%s ", currNode->data);
        }

        currNode = currNode->next;
    }

    return;
}

// copyList()
// Returns a new List representing the same integer
// sequence as L. The cursor in the new list is undefined,
// regardless of the state of the cursor in L. The state
// of L is unchanged.
List copyList(List L) {
    if (L == NULL) {
        printf("List Error: calling copyList() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }

    List new = newList();
    Node currNode = getFront(L);
    for (int i = 0; i < length(L); i++) {
        append(new, currNode->data);
        currNode = currNode->next;
    }

    return new;
}

// concatList()
// Returns a new List which is the concatenation of
// A and B. The cursor in the new List is undefined,
// regardless of the states of the cursors in A and B.
// The states of A and B are unchanged.
List concatList(List A, List B) {
    if (A == NULL || B == NULL) {
        printf("List Error: calling concatList() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }

    List new = newList();
    Node currNode = getFront(A);

    while (currNode != NULL) {
        append(new, currNode->data);
        currNode = currNode->next;
    }

    currNode = getFront(B);

    while (currNode != NULL) {
        append(new, currNode->data);
        currNode = currNode->next;
    }

    return new;
}
