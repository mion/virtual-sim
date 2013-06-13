#include "memory.h"

#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>

#define NOT_IN_MEMORY -1

struct VirtualPage
{
    int referenced,
        modified,
        last_access;
    unsigned frame_index;
} *p_table;

/* Interno */
int clock_counter;
int num_virtual_pages;
int page_size;
int physical_memory_size;
int num_page_frames;

/* Estatísticas */
int num_writes_to_disk;
int num_page_faults;

/* Retorna o índice de uma página baseado em um endereço lógico. */
unsigned vir_to_p(unsigned addr, unsigned p_size_kb) {
    assert(p_size_kb > 0);

    return addr >> (lg2(p_size_kb) + 10u);
}

int randrange(int n) {
    return rand() % n;
}

void MemoryInit(int p_size_kb, int phys_mem_kb) {
    int i;

    srand(time(NULL));
    num_writes_to_disk = 0;
    num_page_faults = 0 ;
    clock_counter = 0;
    page_size = p_size_kb;
    physical_memory_size = phys_mem_kb;
    num_page_frames = phys_mem_kb / p_size_kb;
    num_virtual_pages = pow2(32 - (lg2(p_size_kb) + 10));
    p_table = (VirtualPage *) malloc(num_virtual_pages * sizeof(VirtualPage));
    mcheck(p_table); 

    for (i = 0; i < num_virtual_pages; i++) {
        p_table[i].referenced = p_table[i].modified = FALSE; 
        p_table[i].last_access = 0;
        p_table[i].frame_index = NOT_IN_MEMORY;
    }
}

void MemoryClockInterrupt(void) {
    for (i = 0; i < num_virtual_pages; i++) {
        p_table[i].referenced =  FALSE;       
    }
}

int choose_page_frame(void) {
    return randrange(num_page_frames);
}

void evict_page(int frame_index) {
    int i;

    for (i = 0; i < num_virtual_pages; i++) {
        if (p_table[i].frame_index == frame_index) {
            p_table[i].frame_index = NOT_IN_MEMORY;
            return;
        }
    }
}

void MemoryRead(unsigned addr) {
    int index = addr >> (lg2(page_size) + 10);

    p_table[index].referenced = TRUE;

    /* Page fault? */
    if (p_table[index].frame_index == NOT_IN_MEMORY) {
        int f_index = choose_page_frame();
        evict_page(f_index);

        if (p_table[index].modified) {    
            num_writes_to_disk++; /* Coloca o quadro de página no disco. */
        }
        
        p_table[index].frame_index = f_index; 
    }
}

void MemoryWrite(unsigned addr) {

}
