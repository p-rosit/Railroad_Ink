#ifndef GENERATE_TYPE_DATA_H
#define GENERATE_TYPE_DATA_H

#include <stdint.h>
#include "../utils/debug_utils.h"
#include "data_utils.h"

type_data_t* generate_type_data(uint8_t, uint8_t, uint8_t*);
void         find_type_combinations(type_data_t*, uint8_t, uint8_t, uint8_t*);
size_t       generate_combinations_of_n_types(type_data_t*, size_t, size_t*, int, int, uint8_t, uint8_t*, uint8_t, uint8_t*, temp_type_hash_t*); 
uint16_t*    count_type_combinations(uint8_t, uint8_t, uint8_t*);
uint16_t     combinations_of_n_types(int, int, uint8_t, uint8_t*);
type_data_t* allocate_type_data(uint8_t, uint16_t*);

type_data_t* generate_type_data(uint8_t max_combinations, uint8_t expansion_amount, uint8_t* types) {
    int i, j, k;
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
    find_type_combinations(type_data, max_combinations, expansions_with_types, non_zero_types);

    k = 0;
    j = 0;
    DEBUG_PRINT(INFO, "The following combinations of tile types were%cconstructed:\n", ' ');
    DEBUG_PRINT(INFO, "1%ctype:  ", ' ');
    for (i = 0; i < type_data->start_index[max_combinations - 1]; i++) {
        if (i == type_data->start_index[j]) {
            j++;
            DEBUG_PRINT(INFO, "%d types: ", j + 1);
        }

        DEBUG_PRINT(INFO, "%d ", type_data->index2types[i]);
        k += 1;

        if (k > j) {
            if (i + 1 != type_data->start_index[j]) {
                DEBUG_PRINT(INFO, "\n        %c", ' ');
            } else {
                DEBUG_PRINT(INFO, "%c", '\n');
            }
            k = 0;
        }
    }
    DEBUG_PRINT(INFO, "%c", '\n');

    free(non_zero_types);
    free(combinations);
    return type_data;
}

void find_type_combinations(type_data_t* type_data, uint8_t max_combinations, uint8_t expansion_amount, uint8_t* types) {
    size_t ind, type_ind;
    int i, j;
    uint8_t *prev_types, *type_indices, k;
    temp_type_hash_t* temp_map;
    
    prev_types = (uint8_t*) malloc(expansion_amount * sizeof(uint8_t));
    type_indices = (uint8_t*) malloc((expansion_amount + 1) * sizeof(uint8_t));
    temp_map = (temp_type_hash_t*) malloc(sizeof(temp_type_hash_t));

    k = 1;
    for (i = 0; i < expansion_amount; i++) {
        type_indices[i] = k;
        k += types[i];
    }
    type_indices[expansion_amount] = k;

    temp_map->max_hash_size = 10000;
    temp_map->hash_size = 1;
    temp_map->types2index = (uint16_t*) malloc(1 * sizeof(uint16_t));
    temp_map->inds = init_list();
    temp_map->types2index[0] = 65535;
    k = 0;
    add_commutative_pair(temp_map, 1, &k, 0);

    ind = 1;
    type_ind = 1;
    for (i = 0; i < max_combinations; i++) {
        for (j = 0; j < expansion_amount; j++) {
            ind = generate_combinations_of_n_types(
                type_data, ind, &type_ind, i + 1, j, expansion_amount, type_indices, i + 1, prev_types, temp_map
            );
        }
    }

    type_data->types2index = temp_map->types2index;
    type_data->hash_size = temp_map->hash_size;
    
    /*
    uint8_t tt[2] = {2, 4};
    uint8_t ta[1] = {2};
    uint8_t tb[1] = {4};
    uint8_t aa[4] = {1, 3, 6, 9};
    uint8_t ba[3] = {6, 9, 1};
    uint8_t bb[1] = {3};
    printf("%d\n", index_from_types(type_data, 2, tt, 0, NULL));
    printf("%d\n", index_from_types(type_data, 1, ta, 1, tb));
    printf("%d\n", index_from_types(type_data, 4, aa, 0, NULL));
    printf("%d\n", index_from_types(type_data, 3, ba, 1, bb));

    uint8_t* th = (uint8_t*) calloc(4, sizeof(uint8_t));
    uint8_t gg = types_from_index(type_data, 14, th);
    printf("%d %d:", 14, gg);
    for (i = 0; i < gg; i++) printf(" %d", th[i]);
    printf("\n");
    gg = types_from_index(type_data, 80, th);
    printf("%d %d:", 80, gg);
    for (i = 0; i < gg; i++) printf(" %d", th[i]);
    printf("\n");
    free(th);
    */

    free(prev_types);
    free(type_indices);
    //free(temp_map->types2index);
    free_list(temp_map->inds, free);
    free(temp_map);
}

size_t generate_combinations_of_n_types(type_data_t* type_data, size_t ind, size_t* type_ind, int amount, int index, uint8_t expansion_amount, uint8_t* type_indices, uint8_t types_in_combination, uint8_t* prev_types, temp_type_hash_t* temp_map) {
    int i, j;

    if (amount == 1) {
        for (i = type_indices[index]; i < type_indices[index + 1]; i++) {
            prev_types[0] = i;
            for (j = types_in_combination - 1; 0 <= j; j--) {
                type_data->index2types[ind++] = prev_types[j];
            }
            add_commutative_pair(temp_map, types_in_combination, prev_types, (*type_ind)++);
        }
        return ind;
    }

    for (i = index + 1; i < expansion_amount - amount + 2; i++) {
        for (j = type_indices[index]; j < type_indices[index + 1]; j++) {
            prev_types[amount - 1] = j;
            ind = generate_combinations_of_n_types(
                type_data, ind, type_ind, amount - 1, i, expansion_amount, type_indices, types_in_combination, prev_types, temp_map
            );
        }
    }

    return ind;
}

uint16_t* count_type_combinations(uint8_t max_combinations, uint8_t expansion_amount, uint8_t* types) {
    uint16_t* num_combinations;

    DEBUG_PRINT(INFO, "Types in expansions with at least one type:%c", '\n');
    for (int i = 0; i < expansion_amount; i++) DEBUG_PRINT(INFO, "%d ", types[i]);
    DEBUG_PRINT(INFO, "\n%c", '\n');

    num_combinations = (uint16_t*) calloc(max_combinations, sizeof(uint16_t));
    num_combinations[0] += 1;
    for (int i = 0; i < max_combinations; i++) {
        for (int j = 0; j < expansion_amount - i; j++) {
            num_combinations[i] += combinations_of_n_types(i + 1, j, expansion_amount, types);
        }
    }

    DEBUG_PRINT(INFO, "Amount of combinations found for n types:%c", '\n');
    for (int i = 0; i < max_combinations; i++) {
        DEBUG_PRINT(INFO, "(%d => %d) ", i + 1, num_combinations[i]);
    }
    DEBUG_PRINT(INFO, "\n%c", '\n');

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
    uint16_t ind, vec_ind;
    size_t size;
    type_data_t* type_data;

    size = 0;
    for (i = 0; i < max_combinations; i++) {
        size += (i + 1) * combinations[i];
    }

    type_data = (type_data_t*) malloc(sizeof(type_data_t));

    type_data->max_combinations = max_combinations;
    type_data->combination_ind = (uint16_t*) malloc(2 * max_combinations * sizeof(uint16_t));
    type_data->start_index = type_data->combination_ind + max_combinations;
    type_data->index2types = (uint8_t*) calloc(size, sizeof(uint8_t));
    type_data->types2index = NULL;

    ind = 1 * combinations[0];
    vec_ind = 1 * combinations[0];
    for (i = 1; i < max_combinations; i++) {
        type_data->combination_ind[i - 1] = ind;
        type_data->start_index[i - 1] = vec_ind;

        ind += combinations[i];
        vec_ind += (i + 1) * combinations[i];
    }
    type_data->combination_ind[max_combinations - 1] = ind;
    type_data->start_index[max_combinations - 1] = vec_ind;

    return type_data;
}

void free_type_data(type_data_t* type_data) {
    if (type_data == NULL) return;

    free(type_data->combination_ind);
    free(type_data->index2types);
    free(type_data->types2index);
    free(type_data);
}

#endif
