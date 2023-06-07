#ifndef STRING_MAP_H
#define STRING_MAP_H

#include "linked_list.c"
#include "robin_hood.c"
#include <stdint.h>
#include <stdlib.h>

typedef struct string_map       string_map_t;

string_map_t*                   make_string_map(size_t, size_t, linked_list_t*);
string_map_t*                   make_string_map_u8(size_t, size_t, linked_list_t*);
string_map_t*                   make_string_map_u16(size_t, size_t, linked_list_t*);
void                            free_string_map(string_map_t*);

struct string_map {
    string* int2string;
    string vals;
    robin_hash_t* string2int;
};

uint8_t get_mapped_u8(string_map_t* map, string str) {
    return get_val_u8(map->string2int, hash_string(str));
}

uint16_t get_mapped_u16(string_map_t* map, string str) {
    return get_val_u16(map->string2int, hash_string(str));
}

string get_mapped_string(string_map_t* map, uint16_t num) {
    return map->int2string[num];
}

string_map_t* make_string_map_u8(size_t size, size_t max_size, linked_list_t* list) {
    int i;
    string_map_t* map;
    list_element_t* elm;

    map = make_string_map(size, max_size, list);

    for (i = 0, elm = list->frst; elm != NULL; i++, elm = elm->next) {
        add_key_u8(map->string2int, hash_string(elm->data), i);
    }

    return map;
}

string_map_t* make_string_map_u16(size_t size, size_t max_size, linked_list_t* list) {
    int i;
    string_map_t* map;
    list_element_t* elm;

    map = make_string_map(size, max_size, list);

    for (i = 0, elm = list->frst; elm != NULL; i++, elm = elm->next) {
        add_key_u16(map->string2int, hash_string(elm->data), i);
    }

    return map;
}

string_map_t* make_string_map(size_t size, size_t max_size, linked_list_t* list) {
    string_map_t* map;
    size_t num, total_length;
    list_element_t* elm;
    string str, tmp;

    num = 0;
    total_length = 0;
    for (elm = list->frst; elm != NULL; elm = elm->next) {
        str = elm->data;
        while (*(str++) != '\0') {
            total_length += 1;
        }
        total_length += 1;
        num += 1;
    }
    
    map = malloc(sizeof(string_map_t));
    map->int2string = malloc(num * sizeof(string));
    map->vals = malloc(total_length * sizeof(char));
    map->string2int = init_robin_hash(size, max_size);

    num = 0;
    tmp = map->vals;
    for (elm = list->frst; elm != NULL; elm = elm->next) {
        map->int2string[num++] = tmp;

        str = elm->data;
        while (*str != '\0') {
            *(tmp++) = *(str++);
        }
        *(tmp++) = '\0';
    }

    return map;
}

void free_string_map(string_map_t* map) {
    if (map == NULL) return;

    free(map->int2string);
    free(map->vals);
    free_robin_hash(map->string2int);
    free(map);
}

#endif

