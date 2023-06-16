#ifndef STRING_MAP_H
#define STRING_MAP_H

#include "utils.c"
#include "linked_list.c"
#include "hash_map.c"
#include <stdint.h>
#include <stdlib.h>

#define MAKE_STRING_MAP(type) \
 \
typedef struct type##_string_map { \
    string* data2string; \
    string vals; \
    type##_hash_map_t string2data; \
} type##_string_map_t; \
 \
type##_string_map_t new_##type##_string_map(string_list_t* list, size_t initial_size, size_t maximal_size) { \
    size_t i, num, total_length; \
    string_list_element_t* elm; \
    string str, tmp; \
    type##_string_map_t map; \
 \
    num = 0; \
    total_length = 0; \
    for (elm = list->frst; elm != NULL; elm = elm->next) { \
        str = elm->data; \
        while (*(str++) != '\0') { \
            total_length += 1; \
        } \
        total_length += 1; \
        num += 1; \
    } \
 \
    map.data2string = malloc(num * sizeof(string)); \
    map.vals = malloc(total_length * sizeof(char)); \
    map.string2data = type##_new_hash_map(initial_size, maximal_size); \
 \
    num = 0; \
    tmp = map.vals; \
    for (elm = list->frst; elm != NULL; elm = elm->next) { \
        map.data2string[num++] = tmp; \
 \
        str = elm->data; \
        while (*str != '\0') { \
            *(tmp++) = *(str++); \
        } \
        *(tmp++) = '\0'; \
    } \
 \
    for (i = 0, elm = list->frst; elm != NULL; i++, elm = elm->next) { \
        type##_add_hash_val(&map.string2data, hash_string(elm->data), i, 1); \
    } \
 \
    return map; \
} \
type get_string2##type(type##_string_map_t* map, string str) { \
    return *type##_get_hash_val(&map->string2data, hash_string(str)); \
} \
 \
string get_##type##2string(type##_string_map_t* map, type num) { \
    return map->data2string[num]; \
} \
 \
void free_##type##_string_map(type##_string_map_t* map) { \
    free(map->data2string); \
    free(map->vals); \
    type##_free_hash_map(&map->string2data); \
} \
 \

#endif

