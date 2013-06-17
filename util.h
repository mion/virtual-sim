#ifndef UTIL_H_
#define UTIL_H_

#define TRUE 1
#define FALSE 0

#if 0
#define DEBUG
#else
#define DEBUG if(0)
#endif

/* Retorna 2 elevado à n. */
#define pow2(x) (1u<<(x))

/* Retorna log de x na base 2. */
unsigned lg2(unsigned x);

/* Verifica se houve erro de alocação, chamado depois de um malloc. */
void mcheck(void *ptr);

#endif