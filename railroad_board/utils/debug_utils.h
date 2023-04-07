#ifndef DEBUG_UTILS_H
#define DEBUG_UTILS_H

#ifdef DEBUG
# define DEBUG_PRINT(fmt, ...) printf(fmt, __VA_ARGS__);
#else
# define DEBUG_PRINT(fmt, ...) /* Do nothing */
#endif

#include <stdlib.h>
#include "hash_map.h"

void print_hash_map(hash_map_t*);

#ifdef DEBUG

void print_hash_map(hash_map_t* map) {
    printf("map (size: %lu, max_size: %lu): ", map->size, map->max_size);
    for (int i = 0; i < map->key_amount; ++i) {
        printf("%lu ", map->inds[i] % map->size);
    }
    printf("\n");
    for (int i = 0; i < map->key_amount; ++i) {
        printf("%d ", map->vals[map->inds[i] % map->size]);
    }
    printf("\n");
}

#else

void print_hash_map(hash_map_t* map) {

}

#endif
#endif

