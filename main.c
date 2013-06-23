#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "simulator.h"

int main(int argc, char const *argv[]) {
    Options opts;
    Simulator *sim; 

    opts = OptionsFromArgs(argc, argv);
    OptionsPrint(opts);

    sim = SimulatorInit(opts);
    SimulatorRun(sim);
    SimulatorPrintResult(sim);
    SimulatorDestroy(sim);

    return 0;
}
