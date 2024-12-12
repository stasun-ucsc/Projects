#include "shell.h"

#include "gaps.h"

#include <stdint.h>
#include <stdio.h>

/*I received help from Apostolos Mavrogiannakis' section and office hours*/

/*uint32_t array[] = {4, 8, 2, 5, 3, 1, 9, 6, 7, 10};
int array[] = {4, 8, 2, 5, 3};
uint32_t len = 10;

int main(){
	Stats stats;
	shell_sort(&stats, array, len);
}*/

void shell_sort(Stats *stats, uint32_t *arr, uint32_t length) {
    for (uint32_t gap_i = 0; gap_i < GAPS; gap_i++) {
        uint32_t gap = *(gaps + gap_i);
        if ((length - gap) < 1) {
            continue;
        }
        for (uint32_t i = gap; i < length; i++) {
            uint32_t j = i;
            uint32_t tempVar = move(stats, arr[i]);
            while (j >= gap && cmp(stats, tempVar, arr[j - gap]) == -1) {
                *(arr + j) = move(stats, *(arr + j - gap));
                j -= gap;
            }
            *(arr + j) = move(stats, tempVar);
        }
    }
}
