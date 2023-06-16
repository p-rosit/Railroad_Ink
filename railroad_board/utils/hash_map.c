#ifndef HASH_MAP_H
#define HASH_MAP_H

#include <stdlib.h>

size_t _get_max_hash_dist(size_t size) {
    size_t n;
    float dist;

    n = 0;
    dist = 0;
    while (size >>= 1) {
        n += 1;
        dist = n + (size % 2) * dist / 2;
    }
    return dist;
}

typedef struct hash_map {
    size_t max_size;
    size_t max_dist;
    size_t size;
    size_t length;
} hash_map_t;

#define HASH_MAP_FAIL       (0)
#define HASH_MAP_SUCCESS    (1)

#define MAKE_HASH_MAP(type) \
\
typedef struct type##_hash_map_data { \
    size_t dist; \
    size_t hash; \
    type data; \
} type##_hash_map_data_t; \
 \
typedef struct type##_hash_map { \
    size_t max_size; \
    size_t max_dist; \
    size_t size; \
    size_t length; \
    type##_hash_map_data_t* data; \
} type##_hash_map_t; \
 \
type##_hash_map_t type##_new_hash_map(size_t initial_size, size_t maximal_size) { \
    size_t i, max_dist; \
    type##_hash_map_data_t* data; \
 \
    if (maximal_size < initial_size) { \
        return (type##_hash_map_t) { \
            .max_size = 0, \
            .max_dist = 0, \
            .size = 0, \
            .length = 0, \
            .data = NULL \
        }; \
    } \
 \
    max_dist = _get_max_hash_dist(initial_size); \
 \
    if (max_dist == 0) max_dist = 1; \
    data = malloc((initial_size + max_dist) * sizeof(type##_hash_map_data_t)); \
 \
    if (data == NULL) { \
        return (type##_hash_map_t) { \
            .max_size = 0, \
            .max_dist = 0, \
            .size = 0, \
            .length = 0, \
            .data = NULL \
        }; \
    } \
 \
    for (i = 0; i < initial_size + max_dist; i++) { \
        data[i].dist = max_dist; \
    } \
 \
    return (type##_hash_map_t) { \
        .max_size = maximal_size, \
        .max_dist = max_dist, \
        .size = initial_size, \
        .length = 0, \
        .data = data \
    }; \
} \
 \
void type##_free_hash_map(type##_hash_map_t* map) { \
    free(map->data); \
} \
 \
int type##_hash_key_exists(type##_hash_map_t* map, size_t hash) { \
    int key_exists; \
    size_t ind, size, max_dist, dist; \
    type##_hash_map_data_t data; \
 \
    size = map->size; \
    max_dist = map->max_dist; \
    key_exists = HASH_MAP_FAIL; \
 \
    dist = 0; \
    for (ind = hash % size; ind < hash % size + max_dist; dist++, ind++) { \
        data = map->data[ind]; \
        if (data.dist == max_dist) { \
            break; \
        } \
        if (data.hash == hash) { \
            key_exists = HASH_MAP_SUCCESS; \
            break; \
        } \
        if (data.dist < dist) { \
            break; \
        } \
    } \
 \
    return key_exists; \
} \
 \
type* type##_get_hash_val(type##_hash_map_t* map, size_t hash) { \
    size_t ind, size, start, dist; \
    type##_hash_map_data_t elm; \
    type* data; \
 \
    data = NULL; \
    size = map->size; \
    dist = map->max_dist; \
    start = hash % size; \
    for (ind = start; ind < start + dist; ind++) { \
        elm = map->data[ind]; \
 \
        if (elm.dist == dist) { \
            break; \
        } \
        if (elm.hash == hash) { \
            data = &map->data[ind].data; \
            break; \
        } \
        if (elm.dist < ind - start) { \
            break; \
        } \
    } \
 \
    return data; \
} \
 \
int _##type##_add_hash_val(type##_hash_map_t* map, type##_hash_map_data_t* elm) { \
    int added; \
    size_t ind, size; \
    type##_hash_map_data_t temp; \
 \
    size = map->size; \
    elm->dist = 0; \
    for (ind = elm->hash % size; ind < size + map->max_dist; ind++) { \
        if (map->data[ind].dist < elm->dist) { \
            temp = map->data[ind]; \
            map->data[ind] = *elm; \
            *elm = temp; \
        } \
        if (map->data[ind].dist == map->max_dist || map->data[ind].hash == elm->hash) { \
            map->data[ind] = *elm; \
            added = HASH_MAP_SUCCESS; \
            break; \
        } \
 \
        elm->dist += 1; \
 \
        if (elm->dist >= map->max_dist) { \
            added = HASH_MAP_FAIL; \
            break; \
        } \
    } \
 \
    return added; \
} \
 \
int _##type##_resize_hash_map(type##_hash_map_t* map, size_t new_size) { \
    int resized, placed; \
    size_t ind, size, dist, new_dist; \
    type##_hash_map_data_t elm; \
    type##_hash_map_t temp; \
 \
    resized = HASH_MAP_SUCCESS; \
    temp = type##_new_hash_map(new_size, map->max_size); \
    if (temp.data == NULL) return HASH_MAP_FAIL; \
    new_dist = temp.max_dist; \
 \
    for (ind = 0; ind < temp.size + temp.max_dist; ind++) { \
        temp.data[ind].dist = temp.max_dist; \
    } \
 \
    size = map->size; \
    dist = map->max_dist; \
    for (ind = 0; ind < map->size + map->max_dist; ind++) { \
        if (map->data[ind].dist != dist) { \
            elm = map->data[ind]; \
            placed = _##type##_add_hash_val(&temp, &elm); \
            if (!placed) { \
                resized = HASH_MAP_FAIL; \
                break; \
            } \
        } \
    } \
 \
    if (resized) { \
        free(map->data); \
        map->size = new_size; \
        map->max_dist = new_dist; \
        map->data = temp.data; \
    } else {\
        type##_free_hash_map(&temp); \
    } \
 \
    return resized; \
} \
 \
int type##_add_hash_val(type##_hash_map_t* map, size_t hash, type data, size_t increment) { \
    int size_increased, value_added, can_add; \
    size_t inc; \
    type##_hash_map_data_t elm; \
 \
    elm.hash = hash; \
    elm.data = data; \
 \
    inc = increment; \
    size_increased = HASH_MAP_FAIL; \
    value_added = HASH_MAP_FAIL; \
    can_add = HASH_MAP_SUCCESS; \
    while (!can_add || !(value_added = _##type##_add_hash_val(map, &elm))) { \
        if (increment == 0) return HASH_MAP_FAIL; \
 \
        if (map->size + inc > map->max_size) { \
            break; \
        } \
 \
        size_increased = HASH_MAP_SUCCESS; \
        can_add = _##type##_resize_hash_map(map, map->size + inc); \
        if (!can_add) inc += increment; \
    } \
 \
    if (value_added) map->length += 1; \
 \
    return size_increased + value_added; \
} \
 \
int type##_remove_hash_val(type##_hash_map_t* map, size_t hash, double min_fill, size_t increment) { \
    int removed, size_decreased; \
    size_t ind, new_size, size, dist, target_size; \
    double fill; \
    type##_hash_map_data_t elm; \
 \
    removed = HASH_MAP_FAIL; \
    size_decreased = HASH_MAP_FAIL; \
    size = map->size; \
    dist = map->max_dist; \
    for (ind = hash % size; ind < size + dist; ind++) { \
        elm = map->data[ind]; \
        if (elm.dist == dist) { \
            break; \
        } \
        if (removed) { \
            if (elm.dist == 0 || elm.dist == dist) { \
                break; \
            } else { \
                elm.dist -= 1; \
                map->data[ind-1] = elm; \
                map->data[ind].dist = dist; \
            } \
        } \
        if (elm.hash == hash) { \
            map->data[ind].dist = dist; \
            removed = HASH_MAP_SUCCESS; \
        } \
    } \
 \
    if (removed) map->length -= 1; \
 \
    if (removed && map->length > 0) { \
        fill = (double) map->length / (map->size + map->max_dist); \
 \
        if (fill < min_fill) { \
            target_size = map->length / min_fill; \
            for (new_size = target_size; new_size < size; new_size += increment) { \
                if (new_size >= size) { \
                    break; \
                } \
                size_decreased = _##type##_resize_hash_map(map, new_size); \
                if (size_decreased) { \
                    break; \
                } else if (increment == 0) { \
                    break; \
                } \
            } \
        } \
    } \
 \
    return size_decreased + removed; \
} \
 \
size_t type##_next_hash_map_elm(type##_hash_map_t* map, size_t start, size_t* hash, type** data) { \
    size_t exists, ind, dist; \
 \
    dist = map->max_dist; \
    exists = 0; \
    for (ind = start; ind < map->size + map->max_dist; ind++) { \
        if (map->data[ind].dist < dist) { \
            exists = HASH_MAP_SUCCESS; \
            *hash = map->data[ind].hash; \
            *data = &map->data[ind].data; \
            break; \
        } \
    } \
 \
    return exists * (ind + 1); \
} \
 \

#endif
