#include "util.h"

#include <stdlib.h>
#include <assert.h>

int __debug_mode;

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

void print_header(const char *str) {
    printf("---[ %s ]---\n", str);
}

void set_debug_mode(int debug_mode) {
    __debug_mode = debug_mode;
}

int get_debug_mode(void) {
    return __debug_mode;
}