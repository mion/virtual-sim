#include "simulator.h"
#include "memory.h"
#include "util.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

/********** ESTRUTURAS DE DADOS **********/

typedef struct MemAccess {
    char rw;
    unsigned addr;
} MemAccess;

struct Simulator {
    MemAccess mem_accesses[MEM_ACCESS_MAX];
    char alg[31];
    int length, p_size_kb, phys_mem_kb, debug_mode;
    Memory *mem;
};

/********** FUNÇÕES INTERNAS/AUXILIARES **********/

Algorithm AlgorithmFromString(char *str) {
    assert(str);

    if (strcmp(str, "NRU") == 0) {
        return Algorithm.NRU;
    } else if (strcmp(str, "LRU") == 0) {
        return Algorithm.LRU;
    } else if (strcmp(str, "SEG") == 0) { /* Cuidado: input é em português. */
        return Algorithm.SEC;
    } else if (strcmp(str, "RND") == 0) {
        return Algorithm.RANDOM;
    } else {
        printf("ERRO: algoritmo '%s' de substituicao de paginas invalido.\n", str);
        exit(EXIT_FAILURE);
    }
}

/********** FUNÇÕES PÚBLICAS **********/

Options OptionsFromArgs(int argc, char const *argv[]) {    
    if (argc == 5) {
        Options opts;

        opts.algo = AlgorithmFromString(argv[1]);
        strcpy(opts.filename, argv[2]);
        opts.p_size_kb = atoi(argv[3]);
        opts.phys_mem_kb = atoi(argv[4]);

        if (argc == 6) opts.debug_mode = atoi(argv[5]);
        else opts.debug_mode = 0;

        return opts;
    } else {
        printf("ERRO: numero incorreto de argumentos (%d). Esperava 5 (ou 6).\n", argc);
        exit(EXIT_FAILURE); 
    }
}

Simulator *SimulatorInit(Options opts) {
    Simulator *sim;
    FILE *fp;
    int i = 0;
    unsigned addr;
    char rw;

    sim = (Simulator *) malloc(sizeof(Simulator));
    mcheck(sim);

    fp = (FILE *) fopen(opts.filename, "r");
    fcheck(fp);
    while(fscanf(fp, "%x %c ", &addr, &rw) == 2) {
        if (i >= MEM_ACCESS_MAX) {
            printf("ERROR: ultrapassado numero maximo de linhas no arquivo de log.\n");
            exit(EXIT_FAILURE);
        }
        sim->mem_accesses[i].addr = addr;
        sim->mem_accesses[i].rw = rw;
        assert(rw == 'R' || rw == 'W');
        i++;
    }
    fclose(fp);

    sim->length = i;

    return sim;
}

void SimulatorRun(Simulator *sim, int options) {
    int i;
    Memory *mem  = MemoryInit(sim->p_size_kb, sim->phys_mem_kb);

    DEBUG MemoryPrintFrames(mem);

    for (i = 0; i < sim->length; i++) {
        DEBUG printf("\t\tLinha %d / %d\n\n", i + 1, sim->length);

        MemoryAccess(mem, sim->mem_accesses[i].addr, sim->mem_accesses[i].rw);
        MemoryClockInterrupt(mem);

        DEBUG MemoryPrintFrames(mem);

        DEBUG getchar();
    }
}

void SimulatorPrintResults(Simulator *sim) {
    printf("Escritas ao disco: %d\nPage faults: %d\n", 
        sim->mem->stats.writes_to_disk, 
        sim->mem->stats.page_faults);
}

void SimulatorDestroy(Simulator *sim) {
    MemoryDestroy(sim->mem);
}
