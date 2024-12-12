#include "quick.h"

#include <stdint.h>
#include <stdio.h>

uint32_t partition(Stats *stats, uint32_t *A, uint32_t lo, uint32_t hi);

void quick_sorter(Stats *stats, uint32_t *A, uint32_t lo, uint32_t hi);

/*uint32_t array[] = {4, 8, 2, 5, 3, 1, 9, 6, 7, 10};
int array[] = {4, 8, 2, 5, 3};
uint32_t len = 10;

int main(){
	Stats stats;
	quick_sort(&stats, array, len);
}*/

uint32_t partition(Stats *stats, uint32_t *A, uint32_t lo, uint32_t hi) {
    uint32_t i = lo - 1;
    for (uint32_t j = lo; j < hi; j++) {
        if (cmp(stats, A[j - 1], A[hi - 1]) == -1) {
            i++;
            swap(stats, &A[i - 1], &A[j - 1]);
        }
    }
    swap(stats, &A[i], &A[hi - 1]);
    return (i + 1);
}

void quick_sorter(Stats *stats, uint32_t *A, uint32_t lo, uint32_t hi) {
    if (lo < hi) {
        uint32_t p = partition(stats, A, lo, hi);
        quick_sorter(stats, A, lo, p - 1);
        quick_sorter(stats, A, p + 1, hi);
    }
}

void quick_sort(Stats *stats, uint32_t *A, uint32_t n) {
    quick_sorter(stats, A, 1, n);
}
