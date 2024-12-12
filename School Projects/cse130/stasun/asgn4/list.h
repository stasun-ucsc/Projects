// Adapted from Patrick Tantalo's CSE 101 PA 1 in Spring 2023

//-----------------------------------------------------------------------------
// List.h
// Header file for List ADT
//-----------------------------------------------------------------------------

#ifndef LIST_H_INCLUDE_
#define LIST_H_INCLUDE_

#include "rwlock.h"

// Exported types -------------------------------------------------------------
typedef struct ListObj *List;

// Constructors-Destructors ---------------------------------------------------
List newList(void); // Creates and returns a new empty List.

// Access functions -----------------------------------------------------------
int length(List L); // Returns the number of elements in L.

int cur_index(List L); // Returns index of cursor element if defined, -1 otherwise.

rwlock_t *get(List L); // Returns cursor element of L. Pre: length()>0, index()>=0

rwlock_t *search(List L, char *uri); // new addition for this assignment

// Manipulation procedures ----------------------------------------------------
void moveFront(List L); // If L is non-empty, sets cursor under the front element,
    // otherwise does nothing.

void moveNext(List L); // If cursor is defined and not at back, move cursor one
    // step toward the back of L; if cursor is defined and at
    // back, cursor becomes undefined; if cursor is undefined
    // do nothing

rwlock_t *prepend(List L, char *uri); // Insert new element into L. If L is non-empty,
    // insertion takes place before front element.

#endif
