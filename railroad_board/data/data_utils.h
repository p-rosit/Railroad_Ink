#ifndef DATA_UTILS_H
#define DATA_UTILS_H
#include <stdint.h>
#include "../utils/utils.h"
#include "../utils/linked_list.h"

typedef struct game_data     game_data_t;
typedef struct tile_data     tile_data_t;
typedef struct single_tile   single_tile_t;
typedef struct tile_textures tile_textures_t;

struct game_data {
    tile_data_t* tiles;
};

struct single_tile {
    uint8_t id;
    uint8_t type;
    uint8_t connections;
    bool station;
};

struct tile_data {
    uint8_t* ids;
    uint8_t* types;
    uint8_t* connections;
    bool* stations;
};

struct tile_textures {
    uint8_t* textures; // TODO
};

char* concat_string(char* a, char* b) {
    size_t len;
    int i, j;
    char c, *res;

    len = 0; i = 0; j = 0;
    while(a[i++] != '\0') len++;
    while(b[j++] != '\0') len++;
    len += 2;

    res = malloc(len * sizeof(char));

    i = 0; j = 0;
    while ((c = a[i++]) != '\0') res[j++] = c;
    res[j++] = '/';
    i = 0;
    while ((c = b[i++]) != '\0') res[j++] = c;
    res[j++] = '\0';

    return res;
}

tile_data_t* assemble_tile_data(uint8_t amount, linked_list_t** expansion_tiles) {
    return NULL;
}

#endif
