#include "util.h"

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
        exit(1);
    }
}