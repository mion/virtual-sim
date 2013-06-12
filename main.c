#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "simulator.h"
#include "memory.h"

#define ARGV_MAX 101

void parse_args(int argc, 
               char *argv[], 
               char *alg, 
               char *filename_in, 
               int *p_size_kb, 
               int *phys_mem_kb,
               int *debug_mode) {
    if (argc >= 5) {
        strcpy(alg, argv[1]);
        strcpy(filename_in, argv[2]);
        *p_size_kb = atoi(argv[3]);
        *phys_mem_kb = atoi(argv[4]);
        if (argc >= 6) {
            *debug_mode = atoi(argv[5]);
        }
    }
    else {
        printf("ERRO: numero incorreto de argumentos (%d). Esperava 5 (ou 6).\n", argc);
        exit(EXIT_FAILURE); 
    }
}

void init(char *filename_in, Simulator *sim) {
    FILE *fp = (FILE *) fopen(filename_in, "r");
    if (!fp) {
        printf("ERRO: nao foi possivel abrir o arquivo '%s'.\n", filename_in);
        exit(1);
    }
    SimulatorLoad(fp, sim);
    fclose(fp);
}

int main(int argc, char *argv[]) {
    char alg[ARGV_MAX], filename_in[ARGV_MAX];
    int p_size_kb, phys_mem_kb, debug_mode = 0;
    Simulator sim;

    parse_args(argc, argv, alg, filename_in, &p_size_kb, &phys_mem_kb, &debug_mode);

    /* Inicializa simulador com o arquivo de entrada. */
    init(filename_in, &sim);

    printf("Executando o simulador...\n");
    printf("Arquivo de entrada: %s\n", filename_in);
    printf("Tamanho da memoria: %d\n", phys_mem_kb);
    printf("Tamanho das paginas: %d\n", p_size_kb);
    printf("Alg de substituicao: %s\n", alg);
    printf("Numero total de acessos a memoria: %d\n", SimulatorSize(&sim));

    SimulatorPrint(&sim); 

    return 0;
}
