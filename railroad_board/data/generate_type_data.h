#ifndef GENERATE_TYPE_DATA_H
#define GENERATE_TYPE_DATA_H

#include <stdint.h>
#include "data_utils.h"

type_data_t* generate_type_data(uint8_t, uint8_t, uint8_t*);
void         find_type_combinations(type_data_t*, uint8_t, uint8_t*);
uint16_t*    count_type_combinations(uint8_t, uint8_t, uint8_t*);
uint16_t     combinations_of_n_types(int, int, uint8_t, uint8_t*);
type_data_t* allocate_type_data(uint8_t, uint16_t*);

type_data_t* generate_type_data(uint8_t max_combinations, uint8_t expansion_amount, uint8_t* types) {
    int i, j;
    size_t expansions_with_types;
    uint8_t* non_zero_types;
    uint16_t *combinations, *num_combinations;
    type_data_t* type_data;
    
    expansions_with_types = 0;
    for (i = 1; i < expansion_amount; i++) {
        if (types[i] != 0) expansions_with_types += 1;
    }

    j = 0;
    non_zero_types = (uint8_t*) malloc(expansions_with_types * sizeof(uint8_t));
    for (i = 1; i < expansion_amount; i++) {
        if (types[i] != 0) non_zero_types[j++] = types[i];
    }

    combinations = count_type_combinations(max_combinations, expansions_with_types, non_zero_types);
    type_data = allocate_type_data(max_combinations, combinations);
    find_type_combinations(type_data, expansions_with_types, non_zero_types);

    free(non_zero_types);
    free(combinations);
    return type_data;
}

void find_type_combinations(type_data_t* type_data, uint8_t expansion_amount, uint8_t* types) {
    printf("Working on it... ---------------\n");
    return;
}

uint16_t* count_type_combinations(uint8_t max_combinations, uint8_t expansion_amount, uint8_t* types) {
    uint16_t* num_combinations;

    printf("types:");
    for (int i = 0; i < expansion_amount; i++) printf(" %d", types[i]);
    printf("\n");

    num_combinations = (uint16_t*) calloc(max_combinations, sizeof(uint16_t));
    num_combinations[0] += 1;
    for (int i = 0; i < max_combinations; i++) {
        for (int j = 0; j < expansion_amount - i; j++) {
            num_combinations[i] += combinations_of_n_types(i + 1, j, expansion_amount, types);
        }
    }

    printf("comb:");
    for (int i = 0; i < max_combinations; i++) {
        printf(" %d", num_combinations[i]);
    }
    printf("\n");

    return num_combinations;
}

uint16_t combinations_of_n_types(int amount, int ind, uint8_t expansion_amount, uint8_t* types) {
    uint16_t total_combinations;

    if (amount == 1) return types[ind];

    total_combinations = 0;
    for (int i = ind + 1; i < expansion_amount - amount + 2; i++) {
        total_combinations += combinations_of_n_types(amount-1, i, expansion_amount, types);
    }

    return types[ind] * total_combinations;
}

type_data_t* allocate_type_data(uint8_t max_combinations, uint16_t* combinations) {
    int i;
    size_t size;
    type_data_t* type_data;

    size = 0;
    for (i = 0; i < max_combinations; i++) {
        size += (i + 1) * combinations[i];
    }

    type_data = (type_data_t*) malloc(sizeof(type_data_t));

    type_data->max_combinations = max_combinations;
    type_data->combination_ind = (uint16_t*) malloc((max_combinations - 1) * sizeof(uint16_t));
    type_data->index2types = (uint8_t*) malloc(size * sizeof(uint8_t));
    type_data->types2index = NULL;

    return type_data;
}

void free_type_data(type_data_t* type_data) {
    if (type_data == NULL) return;

    free(type_data->combination_ind);
    free(type_data->index2types);
    free(type_data);
}

#endif