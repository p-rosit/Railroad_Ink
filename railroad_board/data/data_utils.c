#ifndef DATA_UTILS_H
#define DATA_UTILS_H

#define IDENTIFIER_SIZE (20)
#define NAME_SIZE       (20)

#include <stdio.h>
#include <stdint.h>

#include "../utils/utils.c"
#include "../utils/debug_utils.c"
#include "../utils/linked_list.c"
#include "../utils/tuple.c"
#include "../utils/string_map.c"

typedef struct temp_meta_data   temp_meta_data_t;
typedef struct temp_tile_data   temp_tile_data_t;

typedef struct settings         settings_t;
typedef struct expansions       expansions_t;
typedef struct name_mapping     name_mapping_t;

// Game data struct
typedef struct game_data        game_data_t;

struct game_data {
    string data_path;
    name_mapping_t* map;
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

struct name_mapping {
    string_map_t* tile;
    string_map_t* connection;
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

void free_name_mapping(game_data_t* game_data) {
    free_string_map(game_data->map->tile);
    free_string_map(game_data->map->connection);
    free(game_data->map);
}

#endif
