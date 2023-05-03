#ifndef DATA_UTILS_H
#define DATA_UTILS_H

#define IDENTIFIER_SIZE (20)
#define NAME_SIZE       (20)

#include <stdio.h>
#include <stdint.h>

#include "../utils/utils.c"
#include "../utils/debug_utils.c"
#include "../utils/linked_list.c"

typedef struct temp_meta_data   temp_meta_data_t;
typedef struct temp_tile_data   temp_tile_data_t;

typedef struct settings         settings_t;
typedef struct expansions       expansions_t;

// Game data struct
typedef struct game_data        game_data_t;

struct game_data {
    string data_path;
    settings_t* settings;
    expansions_t* expansions;
};

struct settings {
    uint8_t max_combinations;
};

struct expansions {
    string* expansions;
    string* expansion_identifiers;
};

struct temp_meta_data {
    int mode;
    bool setting_scope;
    bool expansion_scope;
    linked_list_t* expansion_files;
};

struct temp_tile_data {
    int mode;
    bool tiles_scope;
    bool connections_scope;
    bool traversable_scope;
    bool non_connections_scope;
    bool valid_connections_scope;
    linked_list_t* tile_ids;
    linked_list_t* tile_networks;
    linked_list_t* tile_connections;
    linked_list_t* traversable_connections;
    linked_list_t* non_connections;
    linked_list_t* valid_connections;
};

#endif
