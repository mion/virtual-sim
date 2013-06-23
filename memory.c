#include "memory.h"

#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>

#define NOT_IN_MEMORY -1

/********** ESTRUTURAS DE DADOS **********/

/* Algoritmo de Page Replacement (substituição de página). */
typedef enum {
    RANDOM, /* Escolhe as páginas aleatóriamente. */
    NRU,    /* Non-Recently Used */
    LRU,    /* Least Recently Used */
    SEC     /* Second Chance */
} PRAlgorithm; 

/* Quadro de página
OBS: Na prática, o quadro de página obviamente não 'sabe' qual
o seu índice na tabela de páginas (vir_i).
Implementamos desta forma para melhorar a performance, para
não ser necessário guardar um vetor de 1M de páginas virtuais. */
typedef struct PageFrame {
    int referenced, 
        modified, 
        last_access;
    int vir_i; /* Índice na tabela de páginas virtuais (p_table) */
} PageFrame;

struct Memory {
    PRAlgorithm algo;
    PageFrame *frames;          /* Quadro de páginas */
    int time_counter,           /* Contador de tempo (atualizado a cada clock interrupt) */
        num_virtual_pages,      /* Número de páginas virtuais */
        page_size,              /* Tamanho de uma página em KB */
        physical_memory_size,   /* Tamanho da memória física em KB */
        num_page_frames,        /* Número de quadro de páginas */
        num_used_page_frames;   /* Número de quadro de páginas utilizados (o algoritmo de
                                   substituição só entra em ação quando esse número for
                                   igual à num_page_frames) */
    Statistics stats; /* Estatísticas */
};

/********** FUNÇÕES INTERNAS/AUXILIARES **********/

/* Retorna o índice de uma página baseado em um endereço lógico. */
unsigned vir_to_p(unsigned addr, unsigned p_size_kb) {
    assert(p_size_kb > 0);

    return addr >> (lg2(p_size_kb) + 10u);
}

PRAlgorithm AlgorithmFromString(char *str) {
    assert(str);

    if (strcmp(str, "NRU") == 0) {
        return NRU;
    } else if (strcmp(str, "LRU") == 0) {
        return LRU;
    } else if (strcmp(str, "SEG") == 0) { /* Cuidado: input é em português. */
        return SEC;
    } else if (strcmp(str, "RND") == 0) {
        return RANDOM;
    } else {
        printf("ERRO: algoritmo '%s' de substituicao de paginas invalido.\n", str);
        exit(EXIT_FAILURE);
    }
}

/* Escolhe uma página para ser retirada da memória. */
int choose_page_frame(Memory *mem, PRAlgorithm algo) {
    int frame_i = -1;

    if (algo == RANDOM)
        frame_i = rand() % mem->num_page_frames; /* Escolhe aleatóriamente. */

    DEBUG printf("Escolhendo frame_i: %d\n", frame_i);

    return frame_i;
}

/* Retira um quadro de página da memória física. */
void evict_page(Memory *mem, int frame_i) {
    assert(0 <= frame_i && frame_i < mem->num_page_frames);

    DEBUG printf("Removendo quadro %d...\n", frame_i);

    mem->frames[frame_i].vir_i = NOT_IN_MEMORY;
    mem->frames[frame_i].referenced = FALSE;
    // frames[frame_i].modified = FALSE;
}

/*  Verifica se o quadro de página com índice frame_i foi modificado e 
    simula a escrita ao disco.
    Nesse caso, para simular a escrita, basta incrementar um contador. */
void check_modified(Memory *mem, int frame_i) {
    DEBUG printf("Quadro de pagina %d foi modificado? ", frame_i); 
    if (mem->frames[frame_i].modified) {
        DEBUG printf("SIM! Incrementando numero de escritas...\n");

        mem->stats.writes_to_disk += 1;

        DEBUG printvar(mem->stats.writes_to_disk);
    }
    DEBUG printf("Nao!\n");
}

/* Simula o carregamento de um quadro de página na memória física. */
void load_page(Memory *mem, int vir_i, int frame_i, char rw) {
    assert_index(vir_i, mem->num_virtual_pages);
    assert_index(frame_i, mem->num_page_frames);

    DEBUG printf("Carregando quadro %d em vir_i=%d, rw=%c\n", frame_i, vir_i, rw);

    mem->frames[frame_i].vir_i = vir_i;

    mem->frames[frame_i].referenced = TRUE;
    if (rw == 'W') {
        mem->frames[frame_i].modified = TRUE;
    }
}

/* Procura o quadro de página com o índice virtual vir_i.
   Caso encontrado, retorna o índice da página no vetor frames.
   Obs: Isto só é necessário na simulação, na prática teríamos um outro vetor 
   representando a tabela de páginas virtuais. */
int find_frame(Memory *mem, int vir_i) {
    int i;

    DEBUG printf("Procurando pagina com vir_i=%d... ", vir_i);

    for (i = 0; i < mem->num_page_frames; i++) {
        if (mem->frames[i].vir_i == vir_i) {
            DEBUG printf("encontrada %d!\n", i);
            return i;
        }
    }

    DEBUG printf("NAO encontrada!\n");

    return NOT_IN_MEMORY;
}

/********** FUNÇÕES PÚBLICAS **********/

Memory *MemoryInit(char *algo, int p_size_kb, int phys_mem_kb) {
    int i;
    Memory *mem = (Memory *) malloc(sizeof(Memory));
    mcheck(mem);

    srand(time(NULL));

    mem->algo = AlgorithmFromString(algo);

    mem->stats.writes_to_disk = 0;
    mem->stats.page_faults = 0 ;

    mem->time_counter = 0;
    mem->num_used_page_frames = 0;

    mem->page_size = p_size_kb;
    mem->physical_memory_size = phys_mem_kb;
    mem->num_page_frames = phys_mem_kb / p_size_kb;
    mem->num_virtual_pages = pow2(32 - (lg2(p_size_kb) + 10));

    mem->frames = (PageFrame *) malloc(mem->num_page_frames * sizeof(PageFrame));
    mcheck(mem->frames);

    for (i = 0; i < mem->num_page_frames; i++) {
        mem->frames[i].referenced = FALSE;
        mem->frames[i].modified = FALSE;
        mem->frames[i].last_access = 0;
        mem->frames[i].vir_i = NOT_IN_MEMORY;
    }

    return mem;
}

void MemoryDestroy(Memory *mem) {
    free(mem->frames);
    free(mem);
}

void MemoryClockInterrupt(Memory *mem) {
    int i;

    mem->time_counter += 1;

    if (mem->time_counter % 2 == 0) {
        DEBUG printf("Limpando bits R\n");

        for (i = 0; i < mem->num_page_frames; i++) {
            mem->frames[i].referenced =  FALSE;       
        }
    }
}

void MemoryAccess(Memory *mem, unsigned addr, char rw) {
    int vir_i, frame_i;

    vir_i = addr >> (lg2(mem->page_size) + 10);
    frame_i = find_frame(mem, vir_i);

    DEBUG printf("Acesso: %x %c\n", addr, rw);

    DEBUG printvar(vir_i);
    DEBUG printvar(frame_i);
    DEBUG printvar(mem->num_used_page_frames);

    /* Page fault? */
    if (frame_i == NOT_IN_MEMORY) {
        DEBUG printf("Page fault!\n"); 

        /* Algoritmo só entra em ação quando a memória física estiver cheia. */
        if (mem->num_used_page_frames < mem->num_page_frames) { 
            load_page(mem, vir_i, mem->num_used_page_frames, rw);
            mem->num_used_page_frames += 1;
        } else {
            int chosen_frame_i; 

            mem->stats.page_faults += 1;

            /* Quando ocorre uma 'page fault', deve-se escolher uma página... */
            chosen_frame_i = choose_page_frame(mem, RANDOM);
            /* ...para ser retirada da memória. */
            evict_page(mem, chosen_frame_i);
            /* Se a página foi modificada, é necessário rescrever-la no HD,
            para atualizar a cópia que estava no disco. */
            check_modified(mem, chosen_frame_i);
            /* Coloca a nova página na memória. */
            load_page(mem, vir_i, chosen_frame_i, rw);

            mem->frames[chosen_frame_i].referenced = TRUE;
        }
    } else {
        DEBUG printf("Page HIT!\n");

        mem->frames[frame_i].referenced = TRUE ;

        if (rw == 'W') {
            mem->frames[frame_i].modified = TRUE;
            DEBUG printf("frames[%d].modified = TRUE\n", frame_i);
        }
    }
}

void MemoryPrintFrames(Memory *mem) {
    int i;

    printf("\n+");
    for (i = 0; i < 50; i++) printf("-");
    printf("\n|\tframe_i\tvir_i\tref\tmod\n");
    for (i = 0; i < mem->num_page_frames; i++) {
        printf("|\t%d\t%d\t%d\t%d\n", i, mem->frames[i].vir_i, 
                                         mem->frames[i].referenced, 
                                         mem->frames[i].modified);
    } 

    printf("+");
    for (i = 0; i < 50; i++) printf("-");
    printf("\n");
}

Statistics MemoryStatistics(Memory *mem) {
    return mem->stats;
}
