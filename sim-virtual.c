#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define ARGV_MAX 501

/////////////////////////////// Util

/////////////////////////////// Memory

/////////////////////////////// Simulator

#define MEM_ACCESS_MAX 2000

typedef struct MemAccess {
    char rw;
    unsigned addr;
} MemAccess;

typedef struct Simulator {
    MemAccess mem_accesses[MEM_ACCESS_MAX];
    char alg[31];
    int length, p_size_kb, phys_mem_kb, debug_mode;
} Simulator;

void SimulatorLoad(FILE *fp, Simulator *sim) {
    int i = 0;
    unsigned addr;
    char rw;

    assert(fp);
    assert(sim);

    while(fscanf(fp, "%x %c ", &addr, &rw) == 2) {
        if (i >= MEM_ACCESS_MAX) {
            printf("ERROR: ultrapassado numero maximo de linhas no arquivo de log.\n");
            exit(EXIT_FAILURE);
        }
        sim->mem_accesses[i].addr = addr;
        sim->mem_accesses[i].rw = rw;
        i++;
    }

    sim->length = i;

    return;
}

/////////////////////////////// main
int main(int argc, char const *argv[])
{
    FILE *fp;
    char alg[ARGV_MAX], filename_in[ARGV_MAX];
    int p_size_kb, phys_mem_kb;
    Simulator sim;

    if (argc >= 5) {
        strcpy(alg, argv[1]);
        strcpy(filename_in, argv[2]);
        p_size_kb = atoi(argv[3]);
        phys_mem_kb = atoi(argv[4]);
    } else {
        printf("ERRO: numero incorreto de argumentos (%d). Esperava 5 (ou 6).\n", argc);
        exit(EXIT_FAILURE); 
    }

    fp = (FILE *) fopen(filename_in, "r");
    if (!fp) {
        printf("ERRO: nao foi possivel abrir o arquivo '%s'.\n", filename_in);
        exit(EXIT_FAILURE);
    }
    SimulatorLoad(fp, &sim);
    fclose(fp);

    printf("--- CONFIGURACAO DE ENTRADA ---\n");
    printf("\tArquivo de entrada: %s\n", filename_in);
    printf("\tTamanho da memoria: %d\n", phys_mem_kb);
    printf("\tTamanho das paginas: %d\n", p_size_kb);
    printf("\tAlg de substituicao: %s\n", alg);
    printf("\tNumero total de acessos a memoria: %d\n", sim.length);
    printf("\n[!] Executando simulacao, por favor aguarde...\n\n");
    printf("--- RESULTADO DA SIMULACAO ---\n");

    return 0;
}
