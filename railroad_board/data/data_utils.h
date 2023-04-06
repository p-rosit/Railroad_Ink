#ifndef DATA_UTILS_H
#define DATA_UTILS_H
#include <stdint.h>

typedef struct game_data     game_data_t;
typedef struct tile_data     tile_data_t;
typedef struct tile_textures tile_textures_t;

struct game_data {
    char** exp_names;
    tile_data_t* tiles;
};

struct tile_data {
    uint8_t* connections;
    uint8_t* default_types;
};

struct tile_textures {
    uint8_t* textures;
};

#endif
