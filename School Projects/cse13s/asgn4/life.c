#include "universe.h"

#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define OPTIONS "tsn:i:o:h"
#define DELAY   50000

typedef uint32_t Set;

uint32_t set_empty(void);

uint32_t set_insert(Set s, uint8_t x);

bool set_member(Set s, uint8_t x);

uint32_t set_empty(void) {
    return 0x00000000;
}

uint32_t set_insert(Set s, uint8_t x) {
    return s | (1 << x);
}

bool set_member(Set s, uint8_t x) {
    if ((s & (1 << x)) == set_empty()) {
        return false;
    } else {
        return true;
    }
}

int main(int argc, char **argv) {
    int opt = 0;
    bool toroidal = false;
    int generations = 100;
    uint32_t flags = set_empty();
    char *input, *output;
    /* char *input = "stdin.txt";
	char *output = "stdout.txt"; */

    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 't': toroidal = true; break;
        case 's': flags = set_insert(flags, 1); break;
        case 'n': generations = atoi(optarg); break;
        case 'i':
            flags = set_insert(flags, 2);
            input = optarg;
            break;
        case 'o':
            flags = set_insert(flags, 3);
            output = optarg;
            break;
        case 'h': flags = set_insert(flags, 0); break;
        default: flags = set_insert(flags, 0);
        }
    }

    if (set_member(flags, 0)) {
        printf("SYNOPSIS\n    Conway's Game of Life\n");
        printf("USAGE\n    ./life tsn:i:o:h\n");
        printf("OPTIONS\n    -t             Create your universe as a toroidal\n");
        printf("    -s             Silent - do not use animate the evolution\n");
        printf("    -n {number}    Number of generations [default: 100]\n");
        printf("    -i {file}      Input file [default: stdin]\n");
        printf("    -o {file}      Output file [default: stdout]\n");
        printf("    -h             Display program help and usage\n");
        return 0;
    }

    int rows, columns;
    FILE *fp = stdin;
    if (set_member(flags, 2)) {
        fp = fopen(input, "r");

        if (fscanf(fp, "%d %d\n", &rows, &columns) != 2) {
            printf("Malformed input.\n");
            return 0;
        } else if (rows < 0 || columns < 0) {
            printf("Malformed input.\n");
            return 0;
        }
    } else {
        if (fscanf(stdin, "%d %d\n", &rows, &columns) != 2) {
            printf("Malformed input.\n");
            return 0;
        } else if (rows < 0 || columns < 0) {
            printf("Malformed input.\n");
            return 0;
        }
    }

    Universe *A = uv_create(rows, columns, toroidal);
    Universe *B = uv_create(rows, columns, toroidal);

    if (uv_populate(A, fp)) {
        Universe *temp;
        for (int gen = 1; gen <= generations; gen++) {
            if (!(set_member(flags, 1))) {
                initscr();
                curs_set(FALSE);
                clear();
            }
            for (uint32_t row_num = 0; row_num < uv_rows(A); row_num++) {
                for (uint32_t col_num = 0; col_num < uv_cols(A); col_num++) {
                    if (uv_get_cell(A, row_num, col_num)) {
                        if (uv_census(A, row_num, col_num) == 2
                            || uv_census(A, row_num, col_num) == 3) {
                            uv_live_cell(B, row_num, col_num);
                            if (!(set_member(flags, 1))) {
                                mvprintw(row_num, col_num, "o");
                            }
                        } else {
                            uv_dead_cell(B, row_num, col_num);
                            if (!(set_member(flags, 1))) {
                                mvprintw(row_num, col_num, " ");
                            }
                        }
                    } else {
                        if (uv_census(A, row_num, col_num) == 3) {
                            uv_live_cell(B, row_num, col_num);
                            if (!(set_member(flags, 1))) {
                                mvprintw(row_num, col_num, "o");
                            }
                        } else {
                            uv_dead_cell(B, row_num, col_num);
                            if (!(set_member(flags, 1))) {
                                mvprintw(row_num, col_num, " ");
                            }
                        }
                    }
                }
            }
            if (!(set_member(flags, 1))) {
                refresh();
                usleep(DELAY);
            }

            temp = A;
            A = B;
            B = temp;
        }
        if (!(set_member(flags, 1))) {
            endwin();
        }

        if (set_member(flags, 2)) {
            fclose(fp);
        } else {
            fclose(stdin);
        }

        if (set_member(flags, 3)) {
            FILE *fp;

            fp = fopen(output, "w");
            uv_print(A, fp);
            fclose(fp);
            uv_delete(A);
            uv_delete(B);
            return 0;
        } else {
            uv_print(A, stdout);
            fclose(stdout);
            uv_delete(A);
            uv_delete(B);
            return 0;
        }
    }

    else {
        printf("Malformed input.\n");
    }
    return 0;
}
