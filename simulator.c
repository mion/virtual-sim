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
    int num_mem_accesses;
    Memory *mem;
};

/********** FUNÇÕES INTERNAS/AUXILIARES **********/

/********** FUNÇÕES PÚBLICAS **********/

Options OptionsFromArgs(int argc, char const *argv[]) {    
    if (argc == 5 || argc == 6) {
        Options opts;

        strcpy(opts.algo, argv[1]);
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

void OptionsPrint(Options opts) {    
    print_header("OPCOES DE SIMULACAO");
    printf("\tArquivo de entrada: %s\n", opts.filename);
    printf("\tTamanho da memoria: %d\n", opts.phys_mem_kb);
    printf("\tTamanho das paginas: %d\n", opts.p_size_kb);
    printf("\tAlg de substituicao: %s\n", opts.algo);
    printf("\tModo de depuracao: %d\n", opts.debug_mode);
}

Simulator *SimulatorInit(Options opts) {
    Simulator *sim;
    FILE *fp;
    int i = 0;
    unsigned addr;
    char rw;

    set_debug_mode(opts.debug_mode);

    sim = (Simulator *) malloc(sizeof(Simulator));
    mcheck(sim);

    fp = (FILE *) fopen(opts.filename, "r");
    fcheck(fp, opts.filename);
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

    sim->num_mem_accesses = i;

    sim->mem = MemoryInit(opts.algo, opts.p_size_kb, opts.phys_mem_kb);

    return sim;
}

void SimulatorRun(Simulator *sim) {
    int i;

    DEBUG MemoryPrintFrames(sim->mem);

    for (i = 0; i < sim->num_mem_accesses; i++) {
        DEBUG printf("\n\n\n\t\tAcesso %d / %d\n\n\t- %x %c\n", 
                        i + 1, 
                        sim->num_mem_accesses,
                        sim->mem_accesses[i].addr,
                        sim->mem_accesses[i].rw);
        DEBUG printf("\t- Paginas escritas: %d\n\t- Paginas lidas: %d\n\n", 
                        MemoryStatistics(sim->mem).writes_to_disk,
                        MemoryStatistics(sim->mem).page_faults);

        MemoryClockInterrupt(sim->mem);
        MemoryAccess(sim->mem, sim->mem_accesses[i].addr, sim->mem_accesses[i].rw);

        DEBUG MemoryPrintFrames(sim->mem);

        DEBUG getchar();
    }
}

void SimulatorPrintResult(Simulator *sim) {
    Statistics stats = MemoryStatistics(sim->mem);

    print_header("RESULTADO");
    printf("\tNumero total de acessos a memoria: %d\n\tPaginas escritas: %d\n\tPaginas lidas: %d\n", 
        sim->num_mem_accesses,
        stats.writes_to_disk, 
        stats.page_faults);
}

void SimulatorDestroy(Simulator *sim) {
    MemoryDestroy(sim->mem);
    free(sim);
}
