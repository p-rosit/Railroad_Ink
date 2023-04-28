#ifndef HASH_MAP_H
#define HASH_MAP_H

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

typedef struct hash_map hash_map_t;

struct hash_map {
    size_t max_size;
    size_t size;
    size_t* inds;
    size_t key_amount;
    uint8_t* vals;
};


hash_map_t*    init_hash_map(size_t, size_t);
size_t         hash_string(char*);
void           add_num(hash_map_t*, char*, uint8_t);
uint8_t        get_num(hash_map_t*, char*);
void           add_key(hash_map_t*, size_t);
void           increase_hash_map_size(hash_map_t*, size_t, uint8_t);
void           free_hash_map(hash_map_t*);

#include "debug_utils.c"

hash_map_t* init_hash_map(size_t initial_size, size_t max_size) {
    hash_map_t* map = (hash_map_t*) malloc(sizeof(hash_map_t));
    map->max_size = max_size;
    map->size = initial_size;

    map->key_amount = 0;
    map->inds = NULL;
    
    map->vals = (uint8_t*) malloc(initial_size * sizeof(uint8_t));
    for (int i = 0; i < initial_size; ++i) map->vals[i] = 255;

    return map;
}

size_t hash_string(char* key) {
    size_t hash;
    char c;

    hash = 5381;
    while ((c = *key++) != '\0') {
        hash = ((hash << 5) + hash) + c;
    }

    return hash;
}

void add_num(hash_map_t* map, char* key, uint8_t num) {
    size_t ind;

    DEBUG_PRINT(0, "(%s) = (%d)\n", key, num);
    #ifdef DEBUG_LEVEL
    if (num == 255) {
        printf("Fatal error: Tried to insert value 255 into hash table. Invalid value.\n");
        exit(1);
    }
    #endif

    ind = hash_string(key);
    if (map->vals[ind % map->size] != 255) {
        increase_hash_map_size(map, ind, num);
    }

    add_key(map, ind);
    map->vals[ind % map->size] = num;
}

uint8_t get_num(hash_map_t* map, char* key) {
    return map->vals[hash_string(key) % map->size];
}

void add_key(hash_map_t* map, size_t ind) {
    size_t* new_inds = (size_t*) realloc(map->inds, (map->key_amount + 1) * sizeof(size_t));

    if (new_inds != NULL) {
        new_inds[map->key_amount] = ind;
    } else {
        new_inds = (size_t*) malloc((map->key_amount + 1) * sizeof(size_t));
        for (int i = 0; i < map->key_amount; ++i) {
            new_inds[i] = map->inds[i];
        }
        new_inds[map->key_amount] = ind;
        free(map->inds);
    }

    map->key_amount += 1;
    map->inds = new_inds;
}

void increase_hash_map_size(hash_map_t* map, size_t ind, uint8_t num) {
    size_t new_size;
    uint8_t* new_vals;

    DEBUG_PRINT(0, "hash_map: increasing at size %lu ", map->size);

    new_size = map->size;
    new_vals = map->vals;
    while (new_size < map->max_size) {
        new_size += 1;
        new_vals = (uint8_t*) malloc(new_size * sizeof(uint8_t));

        for (int i = 0; i < new_size; ++i) {
            new_vals[i] = 255;
        }

        for (int i = 0; i < map->key_amount; i++) {
            if (new_vals[map->inds[i] % new_size] != 255) {
                goto next_size;
            } else {
                new_vals[map->inds[i] % new_size] = map->vals[map->inds[i] % map->size];
            }
        }

        if (new_vals[ind % new_size] == 255) {
            break;
        }

        next_size:
        free(new_vals);
    }

    DEBUG_PRINT(0, "to %lu, entering (%d)\n", new_size, num);

    if (map->max_size <= new_size) {
        printf("Fatal error: hash_map tried to grow past %lu.\n", map->max_size);
        exit(1);
    }

    free(map->vals);

    map->size = new_size;
    map->vals = new_vals;
}

void free_hash_map(hash_map_t* map) {
    if (map == NULL) return;
    free(map->vals);
    free(map->inds);
    free(map);
}

#endif

