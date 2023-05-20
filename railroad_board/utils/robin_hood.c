#ifndef ROBIN_HOOD_H
#define ROBIN_HOOD_H

#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "utils.c"
#include "debug_utils.c"

typedef struct robin_hash       robin_hash_t;
typedef struct hash_element     hash_element_t;
typedef union hash_data         hash_data_t;
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
void                add_key_robin(robin_hash_t*, hash_element_t);
bool                test_add_key(robin_hash_t*, hash_element_t*);
hash_data_t*        get_val_robin(robin_hash_t*, size_t);
void                increase_robin_size(robin_hash_t*, size_t);

void                add_key_u8(robin_hash_t*, size_t, uint8_t);
void                add_key_u16(robin_hash_t*, size_t, uint16_t);
uint8_t             get_val_u8(robin_hash_t*, size_t);
uint16_t            get_val_u16(robin_hash_t*, size_t);

void                free_robin_hash(robin_hash_t*);

robin_hash_t* init_robin_hash(size_t size, size_t max_size) {
    int i;
    robin_hash_t* map;

    map = malloc(sizeof(robin_hash_t));

    map->size = size;
    map->max_size = max_size;
    map->max_dist = log2(size);
    map->data = calloc(size + map->max_dist, sizeof(hash_element_t));
    for (i = 0; i < size + map->max_dist; i++) {
        map->data[i].dist = map->max_dist;
    }

    return map;
}

void add_key_robin(robin_hash_t* map, hash_element_t elm) {
    bool added;

    while (!(added = test_add_key(map, &elm))) {
        if (map->size + 1 > map->max_size) {
            printf("Fatal error: Tried to increase hash map past %lu.\n", map->max_size);
            exit(1);
        }
        increase_robin_size(map, map->size + 1);
    }
}

void increase_robin_size(robin_hash_t* map, size_t updated_size) {
    int i;
    robin_hash_t* new_map;
    hash_element_t elm;
    size_t new_size;
    bool collisions, placed;

    DEBUG_PRINT(INFO, "Increasing size of map from %lu ", map->size);

    new_map = malloc(sizeof(robin_hash_t));
    new_size = updated_size;
    collisions = true;
    while (collisions) {
        //printf("%lu\n", new_size);
        new_map->max_size = map->max_size;
        new_map->max_dist = log2(new_size);
        new_map->size = new_size;
        new_map->data = calloc(new_size + new_map->max_dist, sizeof(hash_element_t));
        for (i = 0; i < new_map->size + new_map->max_dist; i++) {
            new_map->data[i].dist = new_map->max_dist;
        }

        collisions = false;
        for (i = 0; i < map->size + map->max_dist; i++) {
            if (map->data[i].dist != map->max_dist) {
                elm.data = map->data[i].data;
                elm.hash = map->data[i].hash;
                placed = test_add_key(new_map, &elm);
                if (!placed) {
                    collisions = true;
                    break;
                }
            }
        }
        if (!collisions) {
            break;
        }

        new_size += 1;
        free(new_map->data);
    }

    DEBUG_PRINT(INFO, "to %lu.\n", new_map->size);

    free(map->data);
    map->size = new_map->size;
    map->max_dist = new_map->max_dist;
    map->data = new_map->data;
    free(new_map);
}

bool test_add_key(robin_hash_t* map, hash_element_t* data) {
    size_t ind;
    hash_element_t temp_elm;
    int i;
    bool placed;

    data->dist = 0;
    ind = data->hash % map->size;
    for (i = ind; i < map->size + map->max_dist; i++) {
        //printf("- %d, %d\n", data->dist, i);
        if (map->data[i].dist < data->dist) {
            //printf("swap: %d -> %d\n", data->data.u8, map->data[i].data.u8);
            temp_elm.data = map->data[i].data;
            temp_elm.hash = map->data[i].hash;
            temp_elm.dist = map->data[i].dist;

            map->data[i].data = data->data;
            map->data[i].hash = data->hash;
            map->data[i].dist = data->dist;

            data->data = temp_elm.data;
            data->hash = temp_elm.hash;
            data->dist = temp_elm.dist;
        }
        if (map->data[i].dist == map->max_dist) {
            map->data[i].data = data->data;
            map->data[i].hash = data->hash;
            map->data[i].dist = data->dist;
            placed = true;
            //printf("add dist: %d (%d, %d)\n", data->dist, i, data->data.u8);
            break;
        }

        data->dist += 1;

        if (data->dist >= map->max_dist) {
            placed = false;
            break;
        }
    }

    return placed;
}

hash_data_t* get_val_robin(robin_hash_t* map, size_t hash) {
    size_t ind;
    hash_data_t* data;

#ifdef DEBUG_LEVEL
    data = NULL;
#endif
    ind = hash % map->size;
    for (int i = ind; i < ind + map->max_dist; i++) {
        //printf("ind: %d (%d, %d)\n", i, map->data[i].dist, map->max_dist);
        if (map->data[i].dist == map->max_dist || map->data[i].dist < i - ind) {
            break;
        }
        //printf("hash: (%lu, %lu)\n", map->data[i].hash, hash);
        if (map->data[i].hash == hash) {
            data = &map->data[i].data;
            //printf("get dist: %d\n", map->data[i].dist);
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

void free_robin_hash(robin_hash_t* map) {
    if (map == NULL) return;

    free(map->data);
    free(map);
}

void add_key_u8(robin_hash_t* map, size_t hash, uint8_t val) {
    hash_element_t elm;
    elm.hash = hash;
    elm.data.u8 = val;

    add_key_robin(map, elm);
}

void add_key_u16(robin_hash_t* map, size_t hash, uint16_t val) {
    hash_element_t elm;
    elm.hash = hash;
    elm.data.u16 = val;

    add_key_robin(map, elm);
}

uint8_t get_val_u8(robin_hash_t* map, size_t hash) {
    return get_val_robin(map, hash)->u8;
}

uint16_t get_val_u16(robin_hash_t* map, size_t hash) {
    return get_val_robin(map, hash)->u16;
}

#endif

