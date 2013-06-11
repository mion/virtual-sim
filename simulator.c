#include "simulator.h"

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

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

    sim->size = i;

    return;
}

void MemAccessPrint(MemAccess mem_access) {
    printf("0x%08x - %c\n", mem_access.addr, mem_access.rw);
}

void SimulatorPrint(Simulator *sim) {
    int i;

    assert(sim);

    for (i = 0; i < sim->size; i++) {
        MemAccessPrint(sim->mem_accesses[i]);
    }
}
