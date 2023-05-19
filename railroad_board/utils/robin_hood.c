#ifndef ROBIN_HOOD_H
#define ROBIN_HOOD_H

#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "utils.c"

typedef struct robin_hash       robin_hash_t;
typedef struct hash_element     hash_element_t;
typedef union hash_data        hash_data_t;
typedef uint8_t                 dist_t;

struct robin_hash {
    size_t size;
    size_t max_size;
    dist_t max_dist;
    hash_element_t* data;
};

union hash_data {
    uint8_t u8;
    uint16_t u16;
};

struct hash_element {
    size_t hash;
    dist_t dist;
    hash_data_t data;
};

robin_hash_t*       init_robin_hash(size_t, size_t);
void                add_key_robin(robin_hash_t*, size_t, hash_data_t);
hash_data_t*        get_val_robin(robin_hash_t*, size_t);
void                increase_robin_size(robin_hash_t*, size_t, size_t);

void                add_key_u8(robin_hash_t*, size_t, uint8_t);
void                add_key_u16(robin_hash_t*, size_t, uint16_t);
uint8_t             get_val_u8(robin_hash_t*, size_t);
uint16_t            get_val_u16(robin_hash_t*, size_t);

robin_hash_t* init_robin_hash(size_t size, size_t max_size) {
    int i;
    robin_hash_t* map;

    map = malloc(sizeof(robin_hash_t));

    map->size = size;
    map->max_size = max_size;
    map->max_dist = log2(size);
    map->data = calloc(size + map->max_dist, sizeof(hash_element_t));
    for (i = 0; i < map->max_size + map->max_dist; i++) {
        map->data[i].dist = map->max_dist;
    }

    return map;
}

void add_key_robin(robin_hash_t* map, size_t hash, hash_data_t data) {
    size_t ind;
    hash_element_t* prev_elm;
    hash_element_t temp_elm;
    dist_t curr_dist, prev_dist;
    int i;
    bool placed;

    ind = hash % map->size;
    temp_elm.data = data;
    temp_elm.hash = hash;
    temp_elm.dist = 0;
    for (i = 0; i < map->max_dist; i++) {
        if (map->data[ind + i].dist < temp_elm.dist) {
            temp_elm.data = map->data[ind + i].data;
            temp_elm.hash = map->data[ind + i].hash;
            temp_elm.dist = map->data[ind + i].dist;

            map->data[ind + 1].data = t;
        }
        if (map->data[ind + i].dist == map->max_dist) {
            placed = true;
        }

        prev_dist = map->data[ind + i].dist;
        curr_dist += 1;
    }
}

hash_data_t* get_val_robin(robin_hash_t* map, size_t hash) {
    size_t ind;
    hash_data_t* data;

    data = NULL;
    ind = hash % map->size;
    for (int i = ind; i < ind + map->max_dist; i++) {
        if (map->data[i].dist == map->max_dist) {
            break;
        }
        if (map->data[i].hash == hash) {
            data = &map->data[i].data;
            break;
        }
    }
#ifdef DEBUG_LEVEL
    if (data == NULL) {
        printf("Fatal error: Tried to access value which does not exist in hash map.\n");
        exit(1);
    }
#endif

    return data;
}

#endif

