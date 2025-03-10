#include "heap.h"

#include <stdint.h>
#include <stdio.h>

uint32_t max_child(Stats *stats, uint32_t *A, uint32_t first, uint32_t last);

void fix_heap(Stats *stats, uint32_t *A, uint32_t first, uint32_t last);

void build_heap(Stats *stats, uint32_t *A, uint32_t first, uint32_t last);

/*uint32_t array[] = {4, 8, 2, 5, 3, 1, 9, 6, 7, 10};
int array[] = {4, 8, 2, 5, 3};
uint32_t len = 10;

int main(){
	Stats stats;
	heap_sort(&stats, array, len);
}*/

uint32_t max_child(Stats *stats, uint32_t *A, uint32_t first, uint32_t last) {
    uint32_t left = 2 * first;
    uint32_t right = left + 1;
    if ((right <= last) && cmp(stats, A[right - 1], A[left - 1]) == 1) {
        return right;
    }
    return left;
}

void fix_heap(Stats *stats, uint32_t *A, uint32_t first, uint32_t last) {
    int found = 0;
    uint32_t mother = first;
    uint32_t great = max_child(stats, A, mother, last);
    while (mother <= (last / 2) && !found) {
        if (cmp(stats, A[mother - 1], A[great - 1]) == -1) {
            swap(stats, &A[mother - 1], &A[great - 1]);
            mother = great;
            great = max_child(stats, A, mother, last);
        } else {
            found = 1;
        }
    }
}

void build_heap(Stats *stats, uint32_t *A, uint32_t first, uint32_t last) {
    for (uint32_t father = (last / 2); father > (first - 1); father--) {
        fix_heap(stats, A, father, last);
    }
}

void heap_sort(Stats *stats, uint32_t *A, uint32_t n) {
    uint32_t first = 1;
    uint32_t last = n;
    build_heap(stats, A, first, last);
    for (uint32_t leaf = last; leaf > first; leaf--) {
        swap(stats, &A[first - 1], &A[leaf - 1]);
        fix_heap(stats, A, first, leaf - 1);
    }
}
