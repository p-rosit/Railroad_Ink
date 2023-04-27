#ifndef DATA_UTILS_H
#define DATA_UTILS_H

#define IDENTIFIER_SIZE (20)
#define NAME_SIZE       (20)

#include <stdio.h>
#include <stdint.h>

#include "../utils/utils.h"
#include "../utils/debug_utils.h"
#include "../utils/linked_list.h"

typedef struct temp_meta_data   temp_meta_data_t;
typedef struct temp_tile_data   temp_tile_data_t;

typedef struct settings         settings_t;
typedef struct expansions       expansions_t;

// Game data struct
typedef struct game_data        game_data_t;

struct game_data {
    string_t data_path;
    settings_t* settings;
    expansions_t* expansions;
};

struct settings {
    uint8_t max_combinations;
};

struct expansions {
    string_t* expansions;
    string_t* expansion_identifiers;
};

struct temp_meta_data {
    int mode;
    bool_t setting_scope;
    bool_t expansion_scope;
    linked_list_t* expansion_files;
};

struct temp_tile_data {
    int mode;
    bool_t tiles_scope;
    bool_t connections_scope;
    bool_t traversable_scope;
    bool_t non_connections_scope;
    bool_t valid_connections_scope;
    linked_list_t* tile_ids;
    linked_list_t* tile_networks;
};

#endif
