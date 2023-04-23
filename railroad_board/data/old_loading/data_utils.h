#ifndef DATA_UTILS_H
#define DATA_UTILS_H
#include <stdint.h>
#include "../utils/debug_utils.h"
#include "../utils/utils.h"
#include "../utils/linked_list.h"

#include <stdio.h>

typedef struct game_data      game_data_t;
typedef struct tile_info      tile_info_t;
typedef struct tile_data      tile_data_t;
typedef struct single_tile    single_tile_t;
typedef struct type_data      type_data_t;
typedef struct temp_type_hash temp_type_hash_t;
typedef struct tile_textures  tile_textures_t;

struct game_data {
    tile_info_t* info;
    tile_data_t* tiles;
    type_data_t* types;
    tile_textures_t* textures;
};

struct tile_info {
    uint8_t id_amount;
    uint8_t connection_amount;
    bool* networks;
    bool* traversable;
    bool* non_connection;
    bool* valid_connection;
};

struct single_tile {
    uint8_t id;
    uint8_t type;
    uint8_t connections[4];
    bool station;
};

struct tile_data {
    uint8_t expansion_amount;
    uint16_t tile_amount;
    uint16_t* expansion_ind;
    uint8_t* ids;
    uint8_t* types;
    uint8_t* connections;
    uint8_t* stations;
};

struct type_data {
    uint8_t max_combinations;
    uint16_t* combination_ind;
    uint16_t* start_index;
    uint8_t* index2types;
    uint16_t* types2index;
    size_t hash_size;
};

struct temp_type_hash {
    size_t max_hash_size;
    size_t hash_size;
    uint16_t* types2index;
    linked_list_t* inds;
};

struct tile_textures {
    uint8_t* textures; // TODO
};

char*        concat_string(char*, char*);
bool         string_starts_with(char*, char*);
bool*        parse_internal_networks(char*); 
int          parse_single_network(char*, int, bool*);
size_t       commutative_hash(uint8_t, uint8_t*, uint8_t, uint8_t*);
void         add_commutative_pair(temp_type_hash_t*, uint8_t, uint8_t*, uint16_t);
void         increase_temp_map_size(temp_type_hash_t*, size_t);
 
char* concat_string(char* a, char* b) {
    size_t len;
    int i, j;
    char c, *res;

    len = 0; i = 0; j = 0;
    while(a[i++] != '\0') len++;
    while(b[j++] != '\0') len++;
    len += 2;

    res = (char*) malloc(len * sizeof(char));

    i = 0; j = 0;
    while ((c = a[i++]) != '\0') res[j++] = c;
    res[j++] = '/';
    i = 0;
    while ((c = b[i++]) != '\0') res[j++] = c;
    res[j++] = '\0';

    return res;
}

bool string_starts_with(char* sub, char* string) {
    int i;
    bool match;

    i = 0;
    match = true;
    while (sub[i] != '\0') {
        if (sub[i] != string[i]) {
            match = false;
            break;
        }
        i++;
    }

    return match;
}

bool* parse_internal_networks(char* line) {
    int i;
    char c;
    bool* networks;

    networks = (bool*) calloc(8, sizeof(bool));

    i = 0;
    while (line[i++] != '(');
    i = parse_single_network(line, i, networks);
    while ((c = line[i++]) != '(') {
        if (c == '#' || c == '\0') return networks;
    }
    i = parse_single_network(line, i, networks+4);

    return networks;
}

int parse_single_network(char* line, int ind, bool* network) {
    int i, j;
    char word[6];

    while (line[ind] != ')') {
        i = 0;
        while (line[ind] != ' ' && line[ind] != ',' && line[ind] != ')') {
            word[i++] = line[ind];
            ind++;
        }
        word[i] = '\0';

        j = streq(word, "North") + 2 * streq(word, "East") + 3 * streq(word, "South") + 4 * streq(word, "West");
        if (j > 0) {
            network[j - 1] = true;
        } else {
            printf("Unreachable error: %s is an invalid direction.", word);
            exit(1);
        }

        if (line[ind] == ')') break;
        while (line[ind] == ',' || line[ind] == ' ') {
            ind++;
        }
    }

    return ind;
}

uint16_t index_from_types(type_data_t* type_data, uint8_t na, uint8_t* as, uint8_t nb, uint8_t* bs) {
    return type_data->types2index[commutative_hash(na, as, nb, bs) % type_data->hash_size];
}

uint8_t types_from_index(type_data_t* type_data, uint16_t ind, uint8_t* types) {
    int i;
    uint8_t amount_of_types;
    uint16_t combination_index, prev_start_ind, prev_comb_ind;

    amount_of_types = 0;
    prev_start_ind = 0;
    prev_comb_ind = 0;
    while (type_data->combination_ind[amount_of_types] < ind) {
        prev_start_ind = type_data->start_index[amount_of_types];
        prev_comb_ind = type_data->combination_ind[amount_of_types];
        amount_of_types++;
    }
    
    combination_index = prev_start_ind + (amount_of_types + 1) * (ind - prev_comb_ind);

    for (i = 0; i < amount_of_types + 1; i++) {
        types[i] = type_data->index2types[combination_index + i];
    }

    return amount_of_types + 1;
}

size_t commutative_hash(uint8_t na, uint8_t* as, uint8_t nb, uint8_t* bs) {
    size_t hash, temp;
    int i, j;
/*
    printf("(%d:", na);
    for (i = 0; i < na; i++) printf(" %d", as[i]);
    printf(")\n(%d:", nb);
    for (i = 0; i < nb; i++) printf(" %d", bs[i]);
    printf(")\n");
*/

    //hash = 2166136261;
    hash = 7333;
    int primes[10] = {7, 524287, 3010349, 991, 7669, 4241, 23497, 47629, 115249, 35317};
    for (i = 0; i < 10; i++) {
        temp = primes[i] * as[0];
        for (j = 1; j < na; j++) {
            temp ^= primes[i] * as[j];
        }
        for (j = 0; j < nb; j++) {
            temp ^= primes[i] * bs[j];
        }

        hash = (16777619 * hash) ^ temp;
        //hash = ((hash << 5) + hash) + temp;
    }
/*
    hash = 0;
    for (i = 0; i < 3; i++) {
        hash = ((hash << 5) + hash) ^ as[0];
    }
*/
    return hash;
}

void add_commutative_pair(temp_type_hash_t* temp_map, uint8_t na, uint8_t* as, uint16_t index) {
    size_t ind, *saved_index;

    DEBUG_PRINT(DEBUG, "Adding:%c(", ' ');
    for (int i = na; 0 < i; i--) {
        if (i != na) DEBUG_PRINT(DEBUG, ",%c", ' ');
        DEBUG_PRINT(DEBUG, "%d", as[i - 1]);
    }
    DEBUG_PRINT(DEBUG, ") = (%d)\n", index);
/*
    printf("Current:");
    for (list_element_t* elm = temp_map->inds->frst; elm != NULL; elm = elm->next) {
        printf(" %lu", *((size_t*) elm->data));
    }
*/
    ind = commutative_hash(na, as, 0, NULL);
    //printf(" (%lu)\n", ind);
    //printf("index: %lu\n", ind);
    //printf("size: %lu\n", temp_map->hash_size);
    if (temp_map->types2index[ind % temp_map->hash_size] != 65535) {
        increase_temp_map_size(temp_map, ind);
    }
    //printf("new_size: %lu\n\n", temp_map->hash_size);
    //printf("\n\n");

    saved_index = (size_t*) malloc(sizeof(size_t));
    *saved_index = ind;
    append(temp_map->inds, saved_index);
    temp_map->types2index[ind % temp_map->hash_size] = index;
}

void increase_temp_map_size(temp_type_hash_t* temp_map, size_t ind) {
    int i;
    list_element_t* elm;
    size_t new_size, index;
    uint16_t* new_vals;

    DEBUG_PRINT(DEBUG, "temp_hash_map: increasing at size %lu ", temp_map->hash_size);

    new_size = temp_map->hash_size;
    new_vals = temp_map->types2index;
    while (new_size < temp_map->max_hash_size) {
        new_size += 1;
        new_vals = (uint16_t*) malloc(new_size * sizeof(uint16_t));

        for (i = 0; i < new_size; i++) {
            new_vals[i] = 65535;
        }

        //printf("new:");
        elm = temp_map->inds->frst;
        for (i = 0; i < temp_map->inds->size; i++) {
            index = *((size_t*) elm->data);
            //printf(" %lu", index % new_size);
            if (new_vals[index % new_size] != 65535) {
                //printf("\n");
                goto next_temp_size;
            } else {
                new_vals[index % new_size] = temp_map->types2index[index % temp_map->hash_size];
            }
            elm = elm->next;
        }

        if (new_vals[ind % new_size] == 65535) {
            //printf(" last: (%lu)\n", ind % new_size);
            break;
        } else {
            //printf(" last: %lu\n", ind % new_size);
        }

        next_temp_size:
        free(new_vals);
    }

    DEBUG_PRINT(DEBUG, "to %lu\n", new_size);

    if (temp_map->max_hash_size <= new_size) {
        DEBUG_PRINT(FATAL, "Fatal error: temp hash map tried to grow past %lu.\n", temp_map->max_hash_size);
        exit(1);
    }

    free(temp_map->types2index);
    temp_map->hash_size = new_size;
    temp_map->types2index = new_vals;
}

#endif
