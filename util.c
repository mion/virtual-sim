#include "util.h"

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

unsigned lg2(unsigned x) {
    unsigned r = 0u;
    while(x > 1u) {
       x = x >> 1u; 
       r++;
    }
    return r; 
}

void mcheck(void *ptr) {
    if (!ptr) {
        printf("ERRO: falha ao alocar memoria.\n");
        exit(EXIT_FAILURE);
    }
}

void fcheck(FILE *fp, char *filename) {
    if (!fp) {
        printf("ERRO: nao foi possivel abrir o arquivo '%s'.\n", filename);
        exit(EXIT_FAILURE);
    }
}

void assert_bounds(int i, int lower, int upper) {
    assert(lower <= i && i < upper);
}

void assert_index(int i, int size) {
    assert_bounds(i, 0, size);
}