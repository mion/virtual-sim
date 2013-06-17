#include "memory.h"

#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>

#define NOT_IN_MEMORY -1

typedef struct VirtualPage {
    int referenced, modified, last_access;
    int frame_index;
} VirtualPage;

VirtualPage *p_table; /* Page table */

typedef struct PageFrame {
    int v_ind; /* Índice na tabela de páginas virtuais (p_table) */
} PageFrame;

PageFrame *frames;

typedef enum {
    RANDOM,
    NRU 
} PRAlgorithm; /* Algoritmo de Page Replacement (substituição de página). */

/* Interno */
int clock_counter;
int num_virtual_pages;
int page_size;
int physical_memory_size;
int num_page_frames;
int num_used_page_frames;

/* Estatísticas */
int num_writes_to_disk;
int num_page_faults;

/* Retorna o índice de uma página baseado em um endereço lógico. */
unsigned vir_to_p(unsigned addr, unsigned p_size_kb) {
    assert(p_size_kb > 0);

    return addr >> (lg2(p_size_kb) + 10u);
}

void MemoryInit(int p_size_kb, int phys_mem_kb) {
    int i;

    srand(time(NULL));

    num_writes_to_disk = 0;
    num_page_faults = 0 ;
    clock_counter = 0;
    /* Algoritmo só entra em ação depois de todas estarem carregadas. */
    num_used_page_frames = 0;

    page_size = p_size_kb;
    physical_memory_size = phys_mem_kb;
    num_page_frames = phys_mem_kb / p_size_kb;
    num_virtual_pages = pow2(32 - (lg2(p_size_kb) + 10));

    p_table = (VirtualPage *) malloc(num_virtual_pages * sizeof(VirtualPage));
    mcheck(p_table); 

    frames = (PageFrame *) malloc(num_page_frames * sizeof(PageFrame));
    mcheck(frames);

    for (i = 0; i < num_virtual_pages; i++) {
        p_table[i].referenced = p_table[i].modified = FALSE; 
        p_table[i].last_access = 0;
        p_table[i].frame_index = NOT_IN_MEMORY;
    }

    for (i = 0; i < num_page_frames; i++) {
        frames[i].v_ind = NOT_IN_MEMORY;
    }
}

void MemoryDestroy(void) {
    free(p_table); 
    free(frames);
}

void MemoryClockInterrupt(void) {
    int i;

    // for (i = 0; i < num_virtual_pages; i++) {
    //     p_table[i].referenced =  FALSE;       
    // }
}

int choose_page_frame(PRAlgorithm algo) {
    int index = -1;

    if (algo == RANDOM)
        index = rand() % num_page_frames; /* Escolhe aleatóriamente. */

    return index;
}

int evict_page(int frame_index) {
    int i = frames[frame_index].v_ind;

    if (i != NOT_IN_MEMORY) {
        p_table[i].frame_index = NOT_IN_MEMORY;
        frames[frame_index].v_ind = NOT_IN_MEMORY;
        return i;
    } else {
        return -1;
    }
}

void check_modified(int p_index) {
    if (p_table[p_index].modified) {
        num_writes_to_disk += 1;
    }
}

void load_page(int p_index, int f_index, char rw) {
    p_table[p_index].frame_index = f_index;
    frames[f_index].v_ind = p_index;

    if (rw == 'W')
        p_table[p_index].modified = TRUE;
}

void MemoryAccess(unsigned addr, char rw) {
    int i = addr >> (lg2(page_size) + 10);

    p_table[i].referenced = TRUE;

    DEBUG printf("Acesso %c em %x, i = %d\n", rw, addr, i);

    /* Page fault? */
    if (p_table[i].frame_index == NOT_IN_MEMORY) {
        /* Algoritmo só entra em ação quando a memória física estiver cheia. */
        if (num_used_page_frames < num_page_frames) { 
            load_page(i, num_used_page_frames, rw);
            num_used_page_frames += 1;
        } else {
            int f_index, p_removed_index;

            num_page_faults += 1;

            /* Quando ocorre uma 'page fault', deve-se escolher uma página... */
            f_index = choose_page_frame(RANDOM);
            /* ...para ser retirada da memória. */
            p_removed_index = evict_page(f_index);

            assert(p_removed_index != -1);

            /* Se a página foi modificada, é necessário rescrever-la no HD,
            para atualizar a cópia que estava no disco. */
            check_modified(p_removed_index);

            /* Coloca a nova página na memória. */
            load_page(i, f_index, rw);
        }
    }
}

void MemoryStatistics(int* n_writes, int* n_pfaults) {
    *n_writes = num_writes_to_disk;
    *n_pfaults = num_page_faults;
}
