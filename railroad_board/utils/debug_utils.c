#ifndef DEBUG_UTILS_H
#define DEBUG_UTILS_H

#ifdef DEBUG_LEVEL
# define DEBUG_PRINT(level, fmt, ...) ((int) level < (int) DEBUG_LEVEL) ? 0 : printf(fmt, __VA_ARGS__);
#else
# define DEBUG_PRINT(level, fmt, ...) /* Do nothing */
#endif

#include <stdlib.h>
#include "hash_map.c"

void print_hash_map(hash_map_t*);

#ifdef DEBUG_LEVEL

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

