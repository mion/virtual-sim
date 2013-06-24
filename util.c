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

        if (empty(q)) {
            printf("[]\n");
            return;
        }

        // printf("l: %d, f: %d, c: %d, e: [", q->elem[q->last], q->elem[q->first], q->count);
        printf("[");

        i=q->first; 
        
        while (i != q->last) {
                printf("%d, ",q->elem[i]);
                i = (i+1) % MAX_QUEUE;
        }

        printf("%d]\n",q->elem[i]);
}

/* Implementação bem simples de uma lista. */

list *list_create(int key, int score) {
    list *l = (list *) malloc(sizeof(list)); 
    mcheck(l);

    l->key = key;
    l->score = score;
    l->tail = NULL;

    return l;
}

list *list_destroy(list *l) {
    if (l) {
        list_destroy(l->tail);
        free(l);
    }
    return NULL;
}

list *list_append(list *l, int key, int score) {
    if (!l) {
        return list_create(key, score); 
    } else {
        l->tail = list_append(l->tail, key, score); 
        return l;
    }
}

list *list_prepend(list *l, int key, int score) {
    list *head = list_create(key, score);
    head->tail = l;

    return head;
}

list *list_last(list *l) {
    while (l && l->tail) l = l->tail;
    return l;
}

list *list_insert(list *l, int key, int score) {
    if (!l) {
        return list_create(key, score);
    } else {
        if (score <= l->score) {
            return list_prepend(l, key, score);
        } else {
            l->tail = list_insert(l->tail, key, score); 
            return l;
        }
    }
}

list *list_remove_first(list *l, int *key, int *score) {
    if (!l) return NULL;
    else { 
        list *head = l->tail;
        *key = l->key;
        *score = l->score;
        free(l);
        return head;
    }
}

int list_ordered(list *l) {
    if (!l) return 1;
    else {
        if (!l->tail) return 1;
        else return (l->score <= l->tail->score) && list_ordered(l->tail);
    } 
}

int list_contains(list *l, int key) {
    if (!l) return 0;
    else {
        if (l->key == key) return 1;
        else return list_contains(l->tail, key);
    }
}

int list_count(list *l) {
    if (!l) return 0;
    else {
        return 1 + list_count(l->tail);
    }
}

void list_print(list *l) {
    if (!l) printf("null\n");
    else {
        printf("%d {%d} -> ", l->key, l->score);
        list_print(l->tail);
    }
}
