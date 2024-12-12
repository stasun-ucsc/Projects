#include <stdio.h>
#include <stdlib.h>
#include "word.h"
#include "code.h"

Word *word_create(uint8_t *syms, uint32_t len) {
    Word *word = (Word *) calloc(1, sizeof(Word));
    if (len > 0) {
        word->syms = (uint8_t *) calloc(len, 1);
    }

    else {
        word->syms = NULL;
        word->len = len;
    }

    for (unsigned int i = 0; i < len; i++) {
        word->syms[i] = syms[i];
    }

    word->len = len;
    return word;
}

Word *word_append_sym(Word *w, uint8_t sym) {
    uint32_t new_len = (w->len) + 1;
    uint8_t syms[new_len];

    if (w->len == 0) {
        return word_create(&sym, new_len);
    }

    for (unsigned int i = 0; i < (w->len); i++) {
        syms[i] = w->syms[i];
    }

    syms[new_len - 1] = sym;

    return word_create(syms, new_len);
}

void word_delete(Word *w) {
    free(w->syms);
    w->syms = NULL;

    free(w);
    w = NULL;

    return;
}

WordTable *wt_create(void) {
    Word **word_table = (Word **) calloc(MAX_CODE + 1, sizeof(Word *));

    uint8_t *empty = { 0 };
    word_table[EMPTY_CODE] = word_create(empty, 0);

    return word_table;
}

void wt_reset(WordTable *wt) {
    if (wt[STOP_CODE] != NULL) {
        word_delete(wt[STOP_CODE]);
        wt[STOP_CODE] = NULL;
    }

    for (unsigned int i = 2; i <= MAX_CODE; i++) {
        if (wt[i]) {
            word_delete(wt[i]);
            wt[i] = NULL;
        }
    }

    return;
}

void wt_delete(WordTable *wt) {
    for (unsigned int i = 0; i <= MAX_CODE; i++) {
        if (wt[i]) {
            word_delete(wt[i]);
            wt[i] = NULL;
        }
    }

    free(wt);
    wt = NULL;

    return;
}
