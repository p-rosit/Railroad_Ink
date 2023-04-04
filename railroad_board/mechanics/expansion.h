#ifndef EXPANSION_H
#define EXPANSION_H
#include <stdlib.h>
#include <stdint.h>

#include "expansion_utils.h"
#include "expansions/standard.h"
#include "expansions/network.h"
#include "expansions/longest.h"
#include "expansions/center.h"
#include "expansions/error.h"
#include "expansions/special.h"
#include "expansions/mountain.h"
#include "expansions/building.h"

expansion_list_t* init_expansion_list(size_t, uint8_t*);
init_func_t       get_init_function(uint8_t);
void              free_expansion_list(expansion_list_t*);

expansion_list_t* init_expansion_list(size_t amount, uint8_t* expansions) {
    expansion_list_t* exp_list;
    init_func_t init_func;

    exp_list = malloc(sizeof(expansion_list_t));

    exp_list->amount = amount;
    exp_list->id = malloc(amount * sizeof(uint8_t));
    exp_list->expansion = malloc(amount * sizeof(void*));

    for (int i = 0; i < amount; ++i) {
        exp_list->id[i] = expansions[i];

        init_func = get_init_function(expansions[i]);
        exp_list->expansion[i] = init_func(exp_list);
    }

    return exp_list;
}

init_func_t get_init_function(uint8_t id) {
    init_func_t func;

    switch (id) {
        case STANDARD:
            func = init_standard_expansion;
            break;
        case LONGEST:
            func = init_longest_expansion;
            break;
        default:
            printf("InvalidIdError: tried to initialize expansion with id %d.", id);
            exit(1);
    }

    return func;
}

void free_expansion_list(expansion_list_t* exp_list) {
    for (int i = 0; i < exp_list->amount; ++i) {
        exp_list->expansion[i]->free_func(exp_list->expansion[i]);
    }

    free(exp_list->id);
    free(exp_list->expansion);
    free(exp_list);
}

#endif
