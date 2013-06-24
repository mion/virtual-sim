#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>

#include "simulator.h"
#include "util.h"

#define MAX 10

//  To shuffle an array a of n elements (indices 0..n-1):
// for i from n − 1 downto 1 do
//      j ← random integer with 0 ≤ j ≤ i
//      exchange a[j] and a[i] 
void shuffle(int *a, int n) {
    int i, j, temp;
    for (i = n - 1; i >= 1; i--) {
        j = rand() % (i + 1);
        temp = a[i];
        a[i] = a[j];
        a[j] = temp;
    }
}

void generate(int *a, int n) {
    int i;

    for (i = 0; i < MAX; i++) 
        a[i] = i;

    shuffle(a, n);
}

void print_arr(int *a, int n) {
    int i;
    printf("[");
    for (i = 0; i < n; i++) printf("%d%s", a[i], i < (n - 1) ? ", " : "");
    printf("]\n");
}

void test_list(void) {
    int i, arr[MAX];
    int k, s;
    list *l = EMPTY_LIST;

    generate(arr, MAX);

    for (i = 0; i < MAX; i++) {
        l = list_create(arr[i], 0);

        assert(l != EMPTY_LIST);
        assert(l->key == arr[i]);
        assert(list_count(l) == 1);
        assert(list_contains(l, arr[i]));

        l = list_destroy(l);

        assert(l == EMPTY_LIST);
    }
    for (i = 0; i < MAX; i++) {
        l = list_insert(l, arr[i], arr[i]);
        assert(list_contains(l, arr[i]));
    }
    assert(list_ordered(l));
    for (i = 0; i < MAX; i++) {
        l = list_remove_first(l, &k, &s);
        assert(list_ordered(l));
    }
    l = list_destroy(l);

    l = list_insert(l, 5, 0);
    l = list_insert(l, 4, 1);
    l = list_insert(l, 1, 2);
    l = list_insert(l, 2, 6);
    l = list_insert(l, 7, 4);
    l = list_insert(l, 0, 5);
    list_print(l);

    l = list_insert(l, 20, 10);
    l = list_insert(l, 25, -1);
    l = list_insert(l, 30, 3);
    list_print(l);

    l = list_remove_first(l, &k, &s);
    s = 15;
    l = list_insert(l, k, s);
    list_print(l);

    printf("count: %d\n", list_count(l));
    printf("last: %d\n", list_last(l)->key);

    l = list_destroy(l);
}

void test_queue(void) {
    queue q;

    init_queue(&q);

    push(&q, 5);
    push(&q, 0);
    push(&q, 1);
    push(&q, 2);

    print_queue(&q);
}

void test_simulator(void) {
    Options opts;
    Simulator * sim;

    strcpy(opts.filename, "file.log");
    strcpy(opts.algo, "LRU");
    opts.p_size_kb = 4;
    opts.phys_mem_kb = 16;
    opts.debug_mode = 1;

    OptionsPrint(opts);

    sim = SimulatorInit(opts);
    SimulatorRun(sim);
    SimulatorPrintResult(sim);
    SimulatorDestroy(sim);
}

int main(int argc, char const *argv[]) {
    srand(time(NULL));

    test_simulator();

    return 0;
}
