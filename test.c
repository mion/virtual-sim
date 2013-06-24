#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "simulator.h"
#include "util.h"

int main(int argc, char const *argv[]) {
    // queue q;

    // init_queue(&q);

    // push(&q, 5);
    // push(&q, 0);
    // push(&q, 1);
    // push(&q, 2);

    // print_queue(&q);

    Options opts;
    Simulator * sim;

    strcpy(opts.filename, "file.log");
    strcpy(opts.algo, "NRU");
    opts.p_size_kb = 4;
    opts.phys_mem_kb = 16;
    opts.debug_mode = 1;

    OptionsPrint(opts);

    sim = SimulatorInit(opts);
    SimulatorRun(sim);
    SimulatorPrintResult(sim);
    SimulatorDestroy(sim);

    return 0;
}
