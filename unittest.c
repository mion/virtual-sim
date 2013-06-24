#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "simulator.h"

int main(int argc, char const *argv[]) {
    Options opts;
    Simulator * sim;

    strcpy(opts.filename, "file.log");
    strcpy(opts.algo, "RND");
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
