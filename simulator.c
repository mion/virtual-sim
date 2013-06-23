#include "simulator.h"
#include "memory.h"
#include "util.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

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

Algorithm AlgorithmFromString(char *str) {
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

Options OptionsFromArgs(int argc, char const *argv[]) {    
    if (argc == 5) {
        Options opts;

        opts.algo = AlgorithmFromString(argv[1]);
        strcpy(opts.filename, argv[2]);
        opts.p_size_kb = atoi(argv[3]);
        opts.phys_mem_kb = atoi(argv[4]);

        if (argc == 6) opts.debug_mode = atoi(argv[5]);

        return opts;
    }
    else {
        printf("ERRO: numero incorreto de argumentos (%d). Esperava 5 (ou 6).\n", argc);
        exit(EXIT_FAILURE); 
    }
}

void MemAccessPrint(MemAccess mem_access) {
    printf("0x%08x - %c\n", mem_access.addr, mem_access.rw);
}

void SimulatorPrint(Simulator *sim) {
    int i;

    assert(sim);

    for (i = 0; i < sim->length; i++) {
        MemAccessPrint(sim->mem_accesses[i]);
    }
}

Simulator *SimulatorInit(FILE *fp) {
    Simulator *sim;
    int i = 0;
    unsigned addr;
    char rw;

    assert(fp);

    sim = (Simulator *) malloc(sizeof(Simulator));
    mcheck(sim);

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

    sim->length = i;

    return sim;
}

int SimulatorLength(Simulator *sim) {
    return sim->length;
}

/*
Extraído de "Operating Systems - Design & Implementation" (livro do MINIX), pág.398:

"The R and M bits can be used to build a simple paging algorithm as follows. When a process
is started up, both page bits for all its pages are set to 0 by the operating system. 
Periodically (e.g., on each clock interrupt), the R bit is cleared, to distinguish pages 
that have not been referenced recently from those that have been.
When a page fault occurs, the operating system inspects all the pages and divides them
into four categories based on the current values of their R and M bits:

Class 0: not referenced, not modified.
Class 1: not referenced, modified.
Class 2: referenced, not modified.
Class 3: referenced, modified.

Although class 1 pages seem, at first glance, impossible, they occur when a class 3 page has its R
bit cleared by a clock interrupt. Clock interrupts do not clear the M bit because this information is
needed to know whether the page has to be rewritten to disk or not. Clearing R but not M leads to
a class 1 page.

The NRU (Not Recently Used) algorithm removes a page at random from the lowest numbered
nonempty class. Implicit in this algorithm is that it is better to remove a modified page that has
not been referenced in at least one clock tick (typically 20 msec) than a clean page that is in
heavy use. The main attraction of NRU is that it is easy to understand, moderately efficient to
implement, and gives a performance that, while certainly not optimal, may be adequate."
*/

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
