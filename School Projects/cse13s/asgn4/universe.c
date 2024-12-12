#include "universe.h"

#include <stdlib.h>

int actually_good_modulo(int x, int y);

struct Universe {
    uint32_t rows;
    uint32_t cols;
    bool **grid;
    bool toroidal;
};

Universe *uv_create(uint32_t rows, uint32_t cols, bool toroidal) {
    Universe *universe = (Universe *) calloc(1, sizeof(Universe));
    bool **grid = (bool **) calloc(rows, sizeof(bool *));
    for (uint32_t row = 0; row < rows; row++) {
        grid[row] = (bool *) calloc(cols, sizeof(bool));
    }
    universe->rows = rows;
    universe->cols = cols;
    universe->grid = grid;
    universe->toroidal = toroidal;
    return universe;
}

void uv_delete(Universe *u) {
    for (uint32_t i = 0; i < u->rows; i++) {
        free(u->grid[i]);
    }
    free(u->grid);
    free(u);
    return;
}

uint32_t uv_rows(Universe *u) {
    return u->rows;
}

uint32_t uv_cols(Universe *u) {
    return u->cols;
}

void uv_live_cell(Universe *u, uint32_t r, uint32_t c) {
    u->grid[r][c] = true;
    return;
}

void uv_dead_cell(Universe *u, uint32_t r, uint32_t c) {
    u->grid[r][c] = false;
    return;
}

bool uv_get_cell(Universe *u, uint32_t r, uint32_t c) {
    if ((0 <= r) && (r < u->rows) && (0 <= c) && (c < u->cols)) {
        return u->grid[r][c];
    } else {
        return false;
    }
}

bool uv_populate(Universe *u, FILE *infile) {
    uint32_t x, y, check;
    while ((check = fscanf(infile, "%d %d\n", &x, &y)) == 2) {
        if (!(x >= 0 && x < u->rows && y >= 0 && y < u->cols)) {
            return false;
        } else {
            uv_live_cell(u, x, y);
        }
    }
    if (check == 1) {
        return false;
    }
    return true;
}

int actually_good_modulo(int x, int y) {
    if (x >= 0) {
        return x % y;
    } else {
        x %= y;
        if (x == 0) {
            return x;
        } else {
            return (x += y);
        }
    }
}

uint32_t uv_census(Universe *u, uint32_t r, uint32_t c) {
    uint32_t neighborCount = 0;

    for (int row = -1; row <= 1; row++) {
        for (int col = -1; col <= 1; col++) {
            if (row != 0 || col != 0) {
                if (u->toroidal) {
                    if (uv_get_cell(u, actually_good_modulo(r + row, u->rows),
                            actually_good_modulo(c + col, u->cols))) {
                        neighborCount++;
                    }
                } else {
                    if (uv_get_cell(u, r + row, c + col)) {
                        neighborCount++;
                    }
                }
            }
        }
    }
    return neighborCount;
}

void uv_print(Universe *u, FILE *outfile) {
    for (uint32_t row = 0; row < (u->rows); row++) {
        for (uint32_t col = 0; col < (u->cols); col++) {
            if (uv_get_cell(u, row, col)) {
                fprintf(outfile, "o");
            } else {
                fprintf(outfile, ".");
            }
        }
        fprintf(outfile, "\n");
    }
    return;
}
