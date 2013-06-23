#ifndef UTIL_H_
#define UTIL_H_

#define TRUE 1
#define FALSE 0

#if 0
#define DEBUG
#else
#define DEBUG if(0)
#endif

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

#endif