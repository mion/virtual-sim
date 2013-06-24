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

/* Implementação 'lightweight' de uma fila. */

void init_queue(queue *q) {
    q->first = 0;
    q->last = MAX_QUEUE-1;
    q->count = 0;
}

int push(queue *q, int x) {
    if (q->count >= MAX_QUEUE)
    { return 0; }
    else {
            q->last = (q->last+1) % MAX_QUEUE;
            q->elem[ q->last ] = x;    
            q->count = q->count + 1;

            return 1;
    }
}

int pop(queue *q) {
    int x;

    if (q->count <= 0) { return 0; }
    else {
            x = q->elem[ q->first ];
            q->first = (q->first+1) % MAX_QUEUE;
            q->count = q->count - 1;
    }

    return(x);
}

int rand_elem(queue *q) {
    return q->elem[ q->first + (rand() % q->count) ];
}

int empty(queue *q) {
    if (q->count <= 0) return (1);
    else return (0);
}

void print_queue(queue *q) {
        int i;

        printf("Last: %d\nFirst: %d\nCount: %d\n", q->elem[q->last], q->elem[q->first], q->count);

        i=q->first; 
        
        while (i != q->last) {
                printf("%d ",q->elem[i]);
                i = (i+1) % MAX_QUEUE;
        }

        printf("%d ",q->elem[i]);
        printf("\n\n");
}
