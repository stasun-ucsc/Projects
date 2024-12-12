#include "batcher.h"
#include "set.h"
#include "shell.h"
#include "stats.h"

#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
/* #include "gaps.h" */
#include "heap.h"
#include "quick.h"

/*I received help from Apostolos Mavrogiannakis' section and office hours*/

#define OPTIONS "Hasbhqn:p:r:"

Stats stats;

int main(int argc, char **argv) {
    int opt = 0;
    int r = 13371453;
    int n = 100, p = 100;
    uint32_t flags = set_empty();

    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'H': flags = set_insert(flags, 0); break;
        case 'a': flags = set_insert(flags, 1); break;
        case 's': flags = set_insert(flags, 2); break;
        case 'b': flags = set_insert(flags, 3); break;
        case 'h': flags = set_insert(flags, 4); break;
        case 'q': flags = set_insert(flags, 5); break;
        case 'n': n = atoi(optarg); break;
        case 'p': p = atoi(optarg); break;
        case 'r': r = atoi(optarg); break;
        default: flags = set_insert(flags, 0);
        }
    }

    if (set_member(flags, 0)) {
        printf("SYNOPSIS\n   A collection of comparison-based sorting algorithms.\n\n");
        printf("USAGE\n   ./sorting_amd64 [-Hasbhqn:p:r:] [-n length] [-p elements] [-r seed]\n\n");
        printf("OPTIONS\n   -H              Display program help and usage.\n");
        printf("   -a              Enable all sorts.\n   -s              Enable Shell Sort.\n");
        printf("   -b              Enable Batcher Sort.\n   -h              Enable Heap Sort.\n");
        printf("   -q              Enable Quick Sort.\n   -n length       Specify number of array "
               "elements (default: 100).\n");
        printf("   -p elements     Specify number of elements to print (default: 100).\n   -r seed "
               "        Specify random seed (default: 13371453).\n");
        return 0;
    }

    uint32_t *array = (uint32_t *) malloc(n * sizeof(uint32_t));
    srandom(r);
    for (int i = 0; i < n; i++) {
        array[i] = rand();
    }

    if (set_member(flags, 1)) {
        shell_sort(&stats, array, n);
        printf(
            "Shell Sort, %d elements, %lu moves, %lu compares\n", n, stats.moves, stats.compares);
        for (int i = 0; i < (p < n ? p : n); i++) {
            if (i % 5 == 4) {
                printf("%13" PRIu32, array[i]);
                printf("\n");
            } else if (i == (p < n ? (p - 1) : (n - 1))) {
                printf("%13" PRIu32, array[i]);
                printf("\n");
            } else {
                printf("%13" PRIu32, array[i]);
                printf("\t");
            }
        }
        reset(&stats);

        batcher_sort(&stats, array, n);
        printf(
            "Batcher Sort, %d elements, %lu moves, %lu compares\n", n, stats.moves, stats.compares);
        for (int i = 0; i < (p < n ? p : n); i++) {
            if (i % 5 == 4) {
                printf("%13" PRIu32, array[i]);
                printf("\n");
            } else if (i == (p < n ? (p - 1) : (n - 1))) {
                printf("%13" PRIu32, array[i]);
                printf("\n");
            } else {
                printf("%13" PRIu32, array[i]);
                printf("\t");
            }
        }
        reset(&stats);

        heap_sort(&stats, array, n);
        printf("Heap Sort, %d elements, %lu moves, %lu compares\n", n, stats.moves, stats.compares);
        for (int i = 0; i < (p < n ? p : n); i++) {
            if (i % 5 == 4) {
                printf("%13" PRIu32, array[i]);
                printf("\n");
            } else if (i == (p < n ? (p - 1) : (n - 1))) {
                printf("%13" PRIu32, array[i]);
                printf("\n");
            } else {
                printf("%13" PRIu32, array[i]);
                printf("\t");
            }
        }
        reset(&stats);

        quick_sort(&stats, array, n);
        printf(
            "Quick Sort, %d elements, %lu moves, %lu compares\n", n, stats.moves, stats.compares);
        for (int i = 0; i < (p < n ? p : n); i++) {
            if (i % 5 == 4) {
                printf("%13" PRIu32, array[i]);
                printf("\n");
            } else if (i == (p < n ? (p - 1) : (n - 1))) {
                printf("%13" PRIu32, array[i]);
                printf("\n");
            } else {
                printf("%13" PRIu32, array[i]);
                printf("\t");
            }
        }
        reset(&stats);
        free(array);
        return 0;
    }

    if (set_member(flags, 2)) {
        shell_sort(&stats, array, n);
        printf(
            "Shell Sort, %d elements, %lu moves, %lu compares\n", n, stats.moves, stats.compares);
        for (int i = 0; i < (p < n ? p : n); i++) {
            if (i % 5 == 4) {
                printf("%13" PRIu32, array[i]);
                printf("\n");
            } else if (i == (p < n ? (p - 1) : (n - 1))) {
                printf("%13" PRIu32, array[i]);
                printf("\n");
            } else {
                printf("%13" PRIu32, array[i]);
                printf("\t");
            }
        }
        reset(&stats);
    }

    if (set_member(flags, 3)) {
        batcher_sort(&stats, array, n);
        printf(
            "Batcher Sort, %d elements, %lu moves, %lu compares\n", n, stats.moves, stats.compares);
        for (int i = 0; i < (p < n ? p : n); i++) {
            if (i % 5 == 4) {
                printf("%13" PRIu32, array[i]);
                printf("\n");
            } else if (i == (p < n ? (p - 1) : (n - 1))) {
                printf("%13" PRIu32, array[i]);
                printf("\n");
            } else {
                printf("%13" PRIu32, array[i]);
                printf("\t");
            }
        }
        reset(&stats);
    }

    if (set_member(flags, 4)) {
        heap_sort(&stats, array, n);
        printf("Heap Sort, %d elements, %lu moves, %lu compares\n", n, stats.moves, stats.compares);
        for (int i = 0; i < (p < n ? p : n); i++) {
            if (i % 5 == 4) {
                printf("%13" PRIu32, array[i]);
                printf("\n");
            } else if (i == (p < n ? (p - 1) : (n - 1))) {
                printf("%13" PRIu32, array[i]);
                printf("\n");
            } else {
                printf("%13" PRIu32, array[i]);
                printf("\t");
            }
        }
        reset(&stats);
    }

    if (set_member(flags, 5)) {
        quick_sort(&stats, array, n);
        printf(
            "Quick Sort, %d elements, %lu moves, %lu compares\n", n, stats.moves, stats.compares);
        for (int i = 0; i < (p < n ? p : n); i++) {
            if (i % 5 == 4) {
                printf("%13" PRIu32, array[i]);
                printf("\n");
            } else if (i == (p < n ? (p - 1) : (n - 1))) {
                printf("%13" PRIu32, array[i]);
                printf("\n");
            } else {
                printf("%13" PRIu32, array[i]);
                printf("\t");
            }
        }
        reset(&stats);
    }

    if (!(set_member(flags, 2) || set_member(flags, 3) || set_member(flags, 4)
            || set_member(flags, 5))) {
        printf("SYNOPSIS\n   A collection of comparison-based sorting algorithms.\n\n");
        printf("USAGE\n   ./sorting_amd64 [-Hasbhqn:p:r:] [-n length] [-p elements] [-r seed]\n\n");
        printf("OPTIONS\n   -H              Display program help and usage.\n");
        printf("   -a              Enable all sorts.\n   -s              Enable Shell Sort.\n");
        printf("   -b              Enable Batcher Sort.\n   -h              Enable Heap Sort.\n");
        printf("   -q              Enable Quick Sort.\n   -n length       Specify number of array "
               "elements (default: 100).\n");
        printf("   -p elements     Specify number of elements to print (default: 100).\n   -r seed "
               "        Specify random seed (default: 13371453).\n");
    }
    free(array);
    return 0;
}
