#include "simulator.h"
#include "memory.h"

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

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

int SimulatorLength(Simulator *sim) {
    return sim->length;
}

/*
The R and M bits can be used to build a simple paging algorithm as follows. When a process is
started up, both page bits for all its pages are set to 0 by the operating system. Periodically (e.g.,
on each clock interrupt), the R bit is cleared, to distinguish pages that have not been referenced
recently from those that have been.
When a page fault occurs, the operating system inspects all the pages and divides them into four
categories based on the current values of their R and M bits:

Class 0: not referenced, not modified.
Class 1: not referenced, modified.
Class 2: referenced, not modified.
Class 3: referenced, modified.

Although class 1 pages seem, at first glance, impossible, they occur when a class 3 page has its R
bit cleared by a clock interrupt. Clock interrupts do not clear the M bit because this information is
needed to know whether the page has to be rewritten to disk or not. Clearing R but not M leads to
a class 1 page.

*/

void SimulatorRun(Simulator *sim, int options) {
    int i;

    MemoryInit(sim->p_size_kb, sim->phys_mem_kb);

    for (i = 0; i < sim->length; i++) {
        int virtual_address = sim->mem_accesses[i].addr;
        char rw = sim->mem_accesses[i].rw; 

        assert(rw == 'R' || rw == 'W');

        MemoryClockInterrupt();
        if (rw == 'R')
            MemoryRead(virtual_address);
        else
            MemoryWrite(virtual_address);
    }
}
