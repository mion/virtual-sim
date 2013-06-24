#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "simulator.h"
#include "util.h"

int main(int argc, char const *argv[]) {
    queue q;
    int i;

    srand(time(NULL));

    init_queue(&q);

    for(i = 0; i < 15; i++) {
        push(&q, rand() % 1000);
    }

    printf("%d\n", pop(&q));

    print_queue(&q);

    printf("Random elem: %d\n", rand_elem(&q));
    // Options opts;
    // Simulator * sim;

    // strcpy(opts.filename, "file.log");
    // strcpy(opts.algo, "RND");
    // opts.p_size_kb = 4;
    // opts.phys_mem_kb = 16;
    // opts.debug_mode = 1;

    // OptionsPrint(opts);

    // sim = SimulatorInit(opts);
    // SimulatorRun(sim);
    // SimulatorPrintResult(sim);
    // SimulatorDestroy(sim);

    return 0;
}
