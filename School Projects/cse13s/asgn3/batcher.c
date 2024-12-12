#include "batcher.h"

#include <stdint.h>
#include <stdio.h>

void comparator(Stats *stats, uint32_t *A, uint32_t x, uint32_t y);

uint32_t bit_length(uint32_t n);

/*uint32_t array[] = {4, 8, 2, 5, 3, 1, 9, 6, 7, 10};

uint32_t array[] = {4, 8, 2, 5, 3};

uint32_t len = 10;

int main(){
	Stats stats;
	batcher_sort(&stats, array, len);
}*/

uint32_t bit_length(uint32_t n) {
    uint32_t len = 0;
    while (n > 0) {
        n = n / 2;
        len++;
    }
    return len;
}

void comparator(Stats *stats, uint32_t *A, uint32_t x, uint32_t y) {
    if (cmp(stats, A[x], A[y]) == 1) {
        swap(stats, &A[x], &A[y]);
    }
    return;
}

void batcher_sort(Stats *stats, uint32_t *A, uint32_t n) {
    if (n == 0) {
        return;
    }
    uint32_t t = bit_length(n);
    uint32_t p = 1 << (t - 1);
    while (p > 0) {
        uint32_t q = 1 << (t - 1);
        uint32_t r = 0;
        uint32_t d = p;
        while (d > 0) {
            for (uint32_t i = 0; i < (n - d); i++) {
                if ((i & p) == r) {
                    comparator(stats, A, i, (i + d));
                }
            }
            d = q - p;
            q >>= 1;
            r = p;
        }
        p >>= 1;
    }
    return;
}
