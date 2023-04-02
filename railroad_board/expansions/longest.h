#ifndef LONGEST_H
#define LONGEST_H
#include <stdlib.h>
#include <stdint.h>
#include "expansion_help.h"

typedef struct longest_expansion {
    expansion_list_t* exp_list;
    free_func_t free_func;
    uint16_t points;
} longest_expansion_t;

expansion_t* init_longest_expansion(expansion_list_t*);
void         free_longest_expansion(void*);

expansion_t* init_longest_expansion(expansion_list_t* exp_list) {
    longest_expansion_t* lng_exp = malloc(sizeof(longest_expansion_t));

    lng_exp->exp_list = exp_list;
    lng_exp->free_func = free_longest_expansion;

    return (expansion_t*) lng_exp;
}

void free_longest_expansion(void* exp) {
    longest_expansion_t* lng_exp = exp;
    free(lng_exp);
}

#endif
