#include "memory.h"

#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>

#define NOT_IN_MEMORY -1

typedef struct PageFrame {
    int referenced, modified, last_access;
    int vir_i; /* Índice na tabela de páginas virtuais (p_table) */
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

    frames = (PageFrame *) malloc(num_page_frames * sizeof(PageFrame));
    mcheck(frames);

    for (i = 0; i < num_page_frames; i++) {
        frames[i].referenced = FALSE;
        frames[i].last_access = 0;
        frames[i].vir_i = NOT_IN_MEMORY;
    }
}

void MemoryDestroy(void) {
    free(frames);
}

void MemoryClockInterrupt(void) {
    int i;

    for (i = 0; i < num_page_frames; i++) {
        frames[i].referenced =  FALSE;       
    }
}

int choose_page_frame(PRAlgorithm algo) {
    int frame_i = -1;

    if (algo == RANDOM)
        frame_i = rand() % num_page_frames; /* Escolhe aleatóriamente. */

    return frame_i;
}

/* Simula a retirada de um quadro de página da memória física. */
void evict_page(int frame_i) {
    assert(0 <= frame_i && frame_i < num_page_frames);

    frames[frame_i].vir_i = NOT_IN_MEMORY;
    frames[frame_i].referenced = FALSE;
    frames[frame_i].modified = FALSE;
}

/*  Verifica se o quadro de página com índice frame_i foi modificado e 
    simula a escrita ao disco.
    Nesse caso, para simular a escrita, basta incrementar um contador. */
void check_modified(int frame_i) {
    if (frames[frame_i].modified) {
        num_writes_to_disk += 1;
    }
}

/* Simula o carregamento de um quadro de página na memória física. */
void load_page(int vir_i, int frame_i, char rw) {
    assert_index(vir_i, num_virtual_pages);
    assert_index(frame_i, num_page_frames);

    frames[frame_i].vir_i = vir_i;

    if (rw == 'W')
        frames[frame_i].modified = TRUE;
}

/* Procura o quadro de página com o índice virtual vir_i.
   Caso encontrado, retorna o índice da página no vetor frames.
   Obs: Isto só é necessário na simulação, na prática teríamos um outro vetor 
   representando a tabela de páginas virtuais. */
int find_frame(int vir_i) {
    int i;

    for (i = 0; i < num_page_frames; i++)
        if (frames[i].vir_i == vir_i) return i;

    return NOT_IN_MEMORY;
}

/* Simula um acesso a memória, tanto de leitura como escrita (indicado por rw). */
void MemoryAccess(unsigned addr, char rw) {
    int vir_i, frame_i;

    vir_i = addr >> (lg2(page_size) + 10);
    frame_i = find_frame(vir_i);

    DEBUG printf("Acesso %c em %x, vir_i = %d\n", rw, addr, vir_i);

    /* Page fault? */
    if (frame_i == NOT_IN_MEMORY) {
        /* Algoritmo só entra em ação quando a memória física estiver cheia. */
        if (num_used_page_frames < num_page_frames) { 
            load_page(vir_i, num_used_page_frames, rw);
            num_used_page_frames += 1;
        } else {
            int chosen_frame_i; 

            num_page_faults += 1;

            /* Quando ocorre uma 'page fault', deve-se escolher uma página... */
            chosen_frame_i = choose_page_frame(RANDOM);
            /* ...para ser retirada da memória. */
            evict_page(chosen_frame_i);
            /* Se a página foi modificada, é necessário rescrever-la no HD,
            para atualizar a cópia que estava no disco. */
            check_modified(chosen_frame_i);
            /* Coloca a nova página na memória. */
            load_page(vir_i, chosen_frame_i, rw);

            frames[chosen_frame_i].referenced = TRUE;
        }
    } else {
        frames[frame_i].referenced = TRUE ;

        if (rw == 'W')
            frames[frame_i].modified = TRUE;
    }
}

void MemoryStatistics(int* n_writes, int* n_pfaults, int* num_vir_pages, int* num_p_frames) {
    *n_writes = num_writes_to_disk;
    *n_pfaults = num_page_faults;
    *num_vir_pages = num_virtual_pages;
    *num_p_frames = num_page_frames;
}
