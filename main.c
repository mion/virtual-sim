#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARGV_MAX 101

/* Retorna 2 elevado à n. */
int pow2(int n) {
    if (n == 0) return 1;
    else return 2 << (n - 1);
}

void parse_args(int argc, 
               char *argv[], 
               char *alg, 
               char *filename_in, 
               int *p_size_kb, 
               int *phys_mem_kb)
{
    if (argc != 5) {
        printf("ERRO: numero de parametros incorreto.\nRecebidos %d, esperava 5.\n", argc);
        exit(1);
    } else {
        strcpy(alg, argv[1]);
        strcpy(filename_in, argv[2]);
        *p_size_kb = atoi(argv[3]);
        *phys_mem_kb = atoi(argv[4]);
    }
}

int main(int argc, char *argv[]) {
    char alg[ARGV_MAX], filename_in[ARGV_MAX];
    int p_size_kb, phys_mem_kb;

    parse_args(argc, argv, alg, filename_in, &p_size_kb, &phys_mem_kb);

    printf("Executando o simulador...\n");
    printf("Arquivo de entrada: %s\n", filename_in);
    printf("Tamanho da memoria: %d\n", phys_mem_kb);
    printf("Tamanho das paginas: %d\n", p_size_kb);
    printf("Alg de substituicao: %s\n", alg);

    return 0;
}
