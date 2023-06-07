#ifndef PREPARE_TYPES_H
#define PREPARE_TYPES_H

#include "../utils/linked_list.c"
#include "data_utils.c"
#include <stdint.h>
#include <stdlib.h>

void                print_types(uint16_t*);
size_t              hash_types(uint16_t*);
size_t              hash_combined_types(uint16_t*, uint16_t*);

void                prepare_types(game_data_t*, temp_expansion_data_t*);
void                count_type_combinations(settings_t*, uint16_t, uint16_t*, uint16_t*);
uint16_t            count_combinations_of_n_types(uint16_t, uint16_t, uint16_t, uint16_t*);
void                make_type_combinations(settings_t*, type_data_t*, uint16_t, uint16_t*);
void                make_combinations_of_n_types(type_data_t*, uint16_t, uint16_t, uint16_t, uint16_t*, uint16_t*, uint16_t*);
void                save_int2type(type_data_t*, uint16_t*, uint16_t*);
void                free_type_data(type_data_t*);

size_t hash_types(uint16_t* types) {
    int i;
    size_t hash;

    hash = 2799;
    for (i = 1; i < *types + 1; i++) {
        hash = (hash << 5) * (hash + types[i]);
    }

    return hash;
}

size_t hash_combined_types(uint16_t* types1, uint16_t* types2) {
    int i, j, k;
    uint16_t total, type, types[MAX_COMBINATIONS + 1];

    total = *types1 + *types2;
    if (total > MAX_COMBINATIONS) {
        printf("Fatal error: Tried to combine types past maximal limit of %d.\n", MAX_COMBINATIONS);
        exit(1);
    }

    j = 1; k = 1;
    *types = total;
    for (i = 1; i < total + 1; i++) {
        if (k > *types2) {
            type = types1[j++];
        } else if (j > *types1) {
            type = types2[k++];
        } else if (types1[j] < types2[k]){
            type = types1[j++];
        } else {
            type = types2[k++];
        }

        types[i] = type;
    }

    return hash_types(types);
}

void print_types(uint16_t* types) {
    int i, n;
    n = *types;
    for (i = 1; i < n + 1; i++) {
        if (i > 1) printf(" ");
        printf("%d", types[i]);
    }
}

void prepare_types(game_data_t* game_data, temp_expansion_data_t* ted) {
    int i;
    uint16_t *combinations, *type_amount, size, *type_ind, total;
    linked_list_t* list;
    list_element_t* elm;
    settings_t* settings;
    type_data_t* type_data;

    settings = game_data->settings;
    type_data = malloc(sizeof(type_data_t));

    combinations = calloc(settings->max_combinations, sizeof(uint16_t));
    type_amount = malloc(ted->types->size * sizeof(uint16_t*));
    type_ind = malloc((ted->types->size + 1) * sizeof(uint16_t));

    i = 0;
    for (elm = ted->types->frst; elm != NULL; elm = elm->next) {
        list = elm->data;
        type_amount[i++] = list->size;
    }

    count_type_combinations(settings, ted->types->size, type_amount, combinations);
    size = 2;
    total = 1;
    for (i = 0; i < settings->max_combinations; i++) {
        total += combinations[i];
        size += (i + 2) * combinations[i];
    }

    type_ind[0] = 1;
    for (i = 1, elm = ted->types->frst; elm != NULL; i++, elm = elm->next) {
        type_ind[i] = type_ind[i - 1] + type_amount[i - 1];
    }

    type_data->total_types = 1;
    type_data->int2type_data = malloc(size * sizeof(uint16_t));
    type_data->int2type = malloc(total * sizeof(uint16_t*));
    type_data->type2int = init_robin_hash(10, 1000);
    
    /* Add empty type */
    type_data->int2type[0] = &(type_data->int2type_data[0]);
    type_data->int2type_data[0] = 1;
    type_data->int2type_data[1] = 0;
    add_key_u16(type_data->type2int, hash_types(type_data->int2type[0]), 0);

    make_type_combinations(settings, type_data, ted->types->size, type_ind);

    DEBUG_PRINT(INFO, "Total amount of types: %lu\n", type_data->total_types);
    DEBUG_PRINT(INFO, "Size of type hash map: %lu\n", type_data->type2int->size);
    free(combinations);
    free(type_amount);
    free(type_ind);

    game_data->types = type_data;
}

void count_type_combinations(settings_t* settings, uint16_t expansion_amount, uint16_t* type_amount, uint16_t* combinations) {
    int i, j;
    
    for (i = 0; i < settings->max_combinations; i++) {
        for (j = 0; j < expansion_amount; j++) {
            combinations[i] += count_combinations_of_n_types(i + 1, j, expansion_amount, type_amount);
        }
    }
}

uint16_t count_combinations_of_n_types(uint16_t amount, uint16_t ind, uint16_t expansion_amount, uint16_t* type_amount) {
    uint16_t total;
    if (amount == 1) return type_amount[ind];

    total = 0;
    for (int i = ind + 1; i < expansion_amount; i++) {
        total += count_combinations_of_n_types(amount - 1, i, expansion_amount, type_amount);
    }
    total *= type_amount[ind];

    return total;
}

void make_type_combinations(settings_t* settings, type_data_t* type_data, uint16_t expansion_amount, uint16_t* type_ind) {
    int i, j, k;
    uint16_t *comb, current_ind;

    current_ind = 2;
    comb = calloc(settings->max_combinations + 2, sizeof(uint16_t));
    for (i = 0; i < settings->max_combinations; i++) {
        for (j = 0; j < expansion_amount; j++) {
            for (k = type_ind[j]; k < type_ind[j + 1]; k++) {
                comb[0] = 0;
                comb[1] = i + 1;
                make_combinations_of_n_types(type_data, j, k, expansion_amount, type_ind, comb, &current_ind);
            }
        }
    }
    
    free(comb);
}

void make_combinations_of_n_types(type_data_t* type_data, uint16_t ind, uint16_t type, uint16_t expansion_amount, uint16_t* type_ind, uint16_t* comb, uint16_t* current_ind) {
    uint16_t *amount, *n;
    n = comb;
    amount = comb + 1;
    
    comb[*n + 2] = type;
    *n += 1;
    if (*n >= *amount) {
       save_int2type(type_data, comb + 1, current_ind);
        add_key_u16(type_data->type2int, hash_types(comb + 1), type_data->total_types);

        *n -= 1;
        type_data->total_types += 1;
        return;
    }

    for (int i = ind + 1; i < expansion_amount; i++) {
        for (int j = type_ind[i]; j < type_ind[i + 1]; j++) {
            make_combinations_of_n_types(type_data, i, j, expansion_amount, type_ind, comb, current_ind);
        }
    }

    *n -= 1;
}

void save_int2type(type_data_t* type_data, uint16_t* types, uint16_t* current_ind) {
    int n, i;
    uint16_t* saved;

    saved = &(type_data->int2type_data[*current_ind]);
    n = *types;
    for (i = 0; i < n + 1; i++) {
        saved[i] = types[i];
    }
    
    type_data->int2type[type_data->total_types] = saved;
    *current_ind += n + 1;
}

void free_type_data(type_data_t* type_data) {
    if (type_data == NULL) return;

    free(type_data->int2type);
    free(type_data->int2type_data);
    free_robin_hash(type_data->type2int);
    free(type_data);
}

#endif
