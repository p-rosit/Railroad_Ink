#ifndef STANDARD_H
#define STANDARD_H
#include <stdlib.h>
#include <stdint.h>
#include "../expansion_utils.h"

typedef struct standard_expansion {
    expansion_list_t* exp_list;
    free_func_t free_func;
    uint16_t points;
} standard_expansion_t;

expansion_t* init_standard_expansion(expansion_list_t*);
void         free_standard_expansion(void*);

expansion_t* init_standard_expansion(expansion_list_t* exp_list) {
    standard_expansion_t* std_exp = malloc(sizeof(standard_expansion_t));

    std_exp->exp_list = exp_list;
    std_exp->free_func = free_standard_expansion;

    return (expansion_t*) std_exp;
}

void free_standard_expansion(void* exp) {
    standard_expansion_t* std_exp = exp;
    free(std_exp);
}

#endif
