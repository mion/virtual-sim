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

    sim.p_size_kb = p_size_kb;
    sim.phys_mem_kb = phys_mem_kb;
    sim.debug_mode = debug_mode;

    printf("--- CONFIGURACAO DE ENTRADA ---\n");
    printf("\tArquivo de entrada: %s\n", filename_in);
    printf("\tTamanho da memoria: %d\n", phys_mem_kb);
    printf("\tTamanho das paginas: %d\n", p_size_kb);
    printf("\tAlg de substituicao: %s\n", alg);
    printf("\tNumero total de acessos a memoria: %d\n", SimulatorLength(&sim));
    printf("\n[!] Executando simulacao, por favor aguarde...\n\n");
    printf("--- RESULTADO DA SIMULACAO ---\n");
    SimulatorRun(&sim, 0);
    SimulatorPrintResults(&sim); 

    return 0;
}
