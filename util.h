#ifndef UTIL_H_
#define UTIL_H_

#include <stdio.h>

#define TRUE 1
#define FALSE 0

/* MODO DE DEPURAÇÃO
Só existe um modo de depuração. */
#define DEBUG if(get_debug_mode())

/* Imprime o nome do int e o seu valor. */
#define printvar(name) (printf("%s = %d\n", #name, name))

/* Retorna 2 elevado à n. */
#define pow2(x) (1u<<(x))

/* Retorna log de x na base 2. */
unsigned lg2(unsigned x);

/* Verifica se houve erro de alocação, chamado depois de um malloc. */
void mcheck(void *ptr);

/* Verifica se foi possível abrir um arquivo. */
void fcheck(FILE *fp, char *filename);

void assert_bounds(int i, int lower, int upper);
void assert_index(int i, int size);

void print_header(const char *str);

void set_debug_mode(int debug_mode);
int get_debug_mode(void);

/* Fila de inteiros para auxiliar em alguns algoritmos. */
#define MAX_QUEUE 50000

typedef struct queue {
    int elem[MAX_QUEUE+1],
        first,
        last, 
        count;
} queue;

void init_queue(queue *q);
int push(queue *q, int x);
int pop(queue *q);
int empty(queue *q);
void print_queue(queue *q);
int rand_elem(queue *q);

/* Lista de inteiros ordenada por um score. */
typedef struct list {
    int key, score;
    struct list *tail;
} list;

#define EMPTY_LIST NULL

list *list_create(int key, int score);
list *list_destroy(list *l);

list *list_append(list *l, int key, int score);
list *list_prepend(list *l, int key, int score);
list *list_insert(list *l, int key, int score);
list *list_remove_first(list *l, int *ket, int *score);

list *list_last(list *l);
int list_ordered(list *l);
int list_contains(list *l, int key);
int list_count(list *l);
void list_print(list *l);

#endif
