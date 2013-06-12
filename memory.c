#include "memory.h"

#include "util.h"
#include <assert.h>
#define IS_PAGE_FAULT(page) (page == NULL)

struct VirtualPage
{
    int r_flag,
        m_flag,
        last_access;
};

/* * * * * * * *
 * vir_to_p
 *
 * Retorna o índice de uma página baseado em um endereço lógico.
 * Por exemplo,
 */
unsigned vir_to_p(unsigned addr, unsigned p_size_kb) {
    assert(p_size_kb > 0);

    return addr >> (lg2(p_size_kb) + 10u);
}
