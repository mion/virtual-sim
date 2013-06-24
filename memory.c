#include "memory.h"

#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>

#define NOT_IN_MEMORY -1

#define CLEAR_INTERVAL 100 /* De quanto em quanto tempo os bits R são limpados. */

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
        last_access,
        first_access;
    int vir_i; /* Índice na tabela de páginas virtuais (p_table) */
} PageFrame;

struct Memory {
    PRAlgorithm algo;
    PageFrame *frames;          /* Quadro de páginas (memória física) */
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

int nru(Memory *mem) {
    int i, k; 
    queue frame_indexes[4]; 

    for (k = 0; k < 4; k++)
        init_queue(&frame_indexes[k]);

    for(i = 0; i < mem->num_page_frames; i++) {
        int class = -1, r = mem->frames[i].referenced, m = mem->frames[i].modified;

        if (r && m)         { class = 3; } 
        else if (r && !m)   { class = 2; }
        else if (!r && m)   { class = 1; }
        else if (!r && !m)  { class = 0; }

        assert(class != -1);
                
        push(&frame_indexes[class], i);
    }

    DEBUG print_queue(&frame_indexes[0]);
    DEBUG print_queue(&frame_indexes[1]);
    DEBUG print_queue(&frame_indexes[2]);
    DEBUG print_queue(&frame_indexes[3]);

    for(k = 0; k < 4; k++) {
        if (!empty(&frame_indexes[k])) {
            return rand_elem(&frame_indexes[k]);
        }
    }

    return -1;
}

int sec(Memory *mem) {
    int i, first_access; 
    list *frames_age_desc = EMPTY_LIST;

    for (i = 0; i < mem->num_page_frames; i++) {
        frames_age_desc = list_insert(frames_age_desc, i, mem->frames[i].first_access);
    }

    DEBUG list_print(frames_age_desc);

    while (1) {
        frames_age_desc = list_remove_first(frames_age_desc, &i, &first_access);

        DEBUG printf("Retirado frame %d da lista, escolhe esse? ", i);

        if (!mem->frames[i].referenced) break;
        else {
            DEBUG printf("Nao, coloca de volta.\n");
            mem->frames[i].first_access = mem->time_counter;
            mem->frames[i].referenced = FALSE;
            frames_age_desc = list_insert(frames_age_desc, i, mem->frames[i].first_access);
        }

        DEBUG list_print(frames_age_desc);
    }

    DEBUG printf("Sim!\n");

    frames_age_desc = list_destroy(frames_age_desc);

    return i;
}

int lru(Memory *mem) {
    int i, last_access;
    list *frames_usage_desc = EMPTY_LIST;

    for (i = 0; i < mem->num_page_frames; i++) {
        frames_usage_desc = list_insert(frames_usage_desc, i, mem->frames[i].last_access);
    }
    DEBUG list_print(frames_usage_desc);

    frames_usage_desc = list_remove_first(frames_usage_desc, &i, &last_access);

    DEBUG printf("Quadro utilizado menos recentemente: %d\n", i);

    frames_usage_desc = list_destroy(frames_usage_desc);

    return i;
}

/* Escolhe uma página para ser retirada da memória. */
int choose_page_frame(Memory *mem) {
    int frame_i = -1;

    if (mem->algo == RANDOM) {
        DEBUG printf("[!] Executando algoritmo 'aleatorio'\n");
        frame_i = rand() % mem->num_page_frames; /* Escolhe aleatóriamente. */
    } else if(mem->algo == NRU) {
        DEBUG printf("[!] Executando algoritmo 'nru'\n");
        frame_i = nru(mem);
    } else if(mem->algo == SEC) {
        DEBUG printf("[!] Executando algoritmo 'segunda chance'\n");
        frame_i = sec(mem);
    } else if(mem->algo == LRU) {        
        DEBUG printf("[!] Executando algoritmo 'lru'\n");
        frame_i = lru(mem);
    }

    DEBUG printf("Escolhido frame_i: %d\n", frame_i);

    assert(frame_i != -1);

    return frame_i;
}

/* Retira um quadro de página da memória física. */
void evict_page(Memory *mem, int frame_i) {
    assert(0 <= frame_i && frame_i < mem->num_page_frames);

    DEBUG printf("Removendo quadro %d...\n", frame_i);

    mem->frames[frame_i].vir_i = NOT_IN_MEMORY;
    mem->frames[frame_i].referenced = FALSE;
    mem->frames[frame_i].modified = FALSE;
    mem->frames[frame_i].last_access = -1;
    mem->frames[frame_i].first_access = -1;    
}

/*  Verifica se o quadro de página com índice frame_i foi modificado e 
    simula a escrita ao disco.
    Nesse caso, para simular a escrita, basta incrementar um contador. */
void check_modified(Memory *mem, int frame_i) {
    DEBUG printf("Quadro de pagina %d tinha sido modificado? ", frame_i); 
    if (mem->frames[frame_i].modified) {
        DEBUG printf("SIM! +1 Pagina escrita\n");

        mem->stats.writes_to_disk += 1;

        DEBUG printvar(mem->stats.writes_to_disk);
    } else { 
        DEBUG printf("NAO! +1 Pagina lida\n"); 
        mem->stats.page_faults += 1;
    } 
}

/* Simula o carregamento de um quadro de página na memória física. */
void load_page(Memory *mem, int vir_i, int frame_i, char rw) {
    assert_index(vir_i, mem->num_virtual_pages);
    assert_index(frame_i, mem->num_page_frames);

    DEBUG printf("Carregando novo quadro %d em vir_i=%d, rw=%c\n", frame_i, vir_i, rw);

    mem->frames[frame_i].vir_i = vir_i;
    mem->frames[frame_i].last_access = mem->time_counter;
    mem->frames[frame_i].first_access = mem->time_counter;

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
            DEBUG printf("frame_i = %d\n", i);
            return i;
        }
    }

    DEBUG printf("nao encontrada!\n");

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
        mem->frames[i].last_access = -1;
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

    if (mem->time_counter % CLEAR_INTERVAL == 0) {
        DEBUG printf("[clock interrupt] LIMPANDO BITS R\n\n");

        for (i = 0; i < mem->num_page_frames; i++) {
            mem->frames[i].referenced =  FALSE;       
        }
    }
}

void MemoryAccess(Memory *mem, unsigned vir_addr, char rw) {
    int vir_i, frame_i;

    vir_i = vir_addr >> (lg2(mem->page_size) + 10);
    frame_i = find_frame(mem, vir_i);

    /* Page fault? */
    if (frame_i == NOT_IN_MEMORY) {
        DEBUG printf("==> PAGE FAULT!\n"); 

        /* Algoritmo só entra em ação quando a memória física estiver cheia. */
        if (mem->num_used_page_frames < mem->num_page_frames) { 
            DEBUG printf("[!] Algoritmo nao entra em acao ainda: %d paginas vazias.\n", mem->num_page_frames - mem->num_used_page_frames);
            load_page(mem, vir_i, mem->num_used_page_frames, rw);
            mem->num_used_page_frames += 1;
        } else {
            int chosen_frame_i; 

            /* Quando ocorre uma 'page fault', deve-se escolher uma página... */
            chosen_frame_i = choose_page_frame(mem);
            /* Se a página foi modificada, é necessário rescrever-la no HD,
            para atualizar a cópia que estava no disco. */
            check_modified(mem, chosen_frame_i);
            /* ...para ser retirada da memória. */
            evict_page(mem, chosen_frame_i);
            /* Coloca a nova página na memória. */
            load_page(mem, vir_i, chosen_frame_i, rw);

            mem->frames[chosen_frame_i].referenced = TRUE;
        }
    } else {
        DEBUG printf("==> Page HIT!\n");

        mem->frames[frame_i].referenced = TRUE ;
        mem->frames[frame_i].last_access = mem->time_counter;

        if (rw == 'W') {
            mem->frames[frame_i].modified = TRUE;
            DEBUG printf("frames[%d].modified = TRUE\n", frame_i);
        }
    }
}

void MemoryPrintFrames(Memory *mem) {
    int i;

    printf("\nContador TEMPO: %d\n+", mem->time_counter);
    for (i = 0; i < 50; i++) printf("-");
    printf("\n|\tframe_i\tvir_i\tref\tmod\tfirst\tlast\n");
    for (i = 0; i < mem->num_page_frames; i++) {
        printf("|\t%d\t%d\t%d\t%d\t%d\t%d\n", i, 
                                         mem->frames[i].vir_i, 
                                         mem->frames[i].referenced, 
                                         mem->frames[i].modified,                                         
                                         mem->frames[i].first_access,
                                         mem->frames[i].last_access);
    } 

    printf("+");
    for (i = 0; i < 50; i++) printf("-");
    printf("\n");
}

Statistics MemoryStatistics(Memory *mem) {
    return mem->stats;
}

int MemoryTime(Memory *mem) {
    return mem->time_counter;
}