#ifndef DATA_UTILS_H
#define DATA_UTILS_H

#include <stdlib.h>
#define IDENTIFIER_SIZE (20)
#define NAME_SIZE       (20)

#include <stdio.h>
#include <stdint.h>

#include "../utils/utils.c"
#include "../utils/debug_utils.c"
#include "../utils/linked_list.c"
#include "../utils/tuple.c"
#include "../utils/string_map.c"

typedef struct temp_meta_data           temp_meta_data_t;
typedef struct temp_tile_data           temp_tile_data_t;
typedef struct temp_expansion_data      temp_expansion_data_t;
typedef struct temp_tile                temp_tile_t;
typedef struct temp_dice                temp_dice_t;

typedef struct settings                 settings_t;
typedef struct expansions               expansions_t;
typedef struct name_mapping             name_mapping_t;
typedef struct dice_data                dice_data_t;
typedef struct dice                     dice_t;

// Game data struct
typedef struct game_data                game_data_t;

struct game_data {
    string data_path;
    name_mapping_t* map;
    settings_t* settings;
    expansions_t* expansions;
    dice_data_t* dice;
};

struct settings {
    uint8_t max_combinations;
};

struct expansions {
    string* expansions;
    string* expansion_identifiers;
};

struct dice_data {
    robin_hash_t* map;
    uint16_t tiles[];
};

struct name_mapping {
    string_map_t* tile;
    string_map_t* connection;
    robin_hash_t* dice;
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

struct temp_expansion_data {
    int mode;
    bool type_scope;
    bool tile_scope;
    bool dice_scope;
    linked_list_t* types;
    linked_list_t* tiles;
    linked_list_t* dice;
    robin_hash_t* identifier2index;
};

struct temp_tile {
    string identifier;
    string id;
    string type;
    string connections[4];
    bool station[2];
};

struct temp_dice {
    string identifier;
    string dice[6];
};

void free_temp_tile(void* data) {
    temp_tile_t* tile = data;

    free(tile->identifier);
    free(tile->id);
    free(tile->type);
    for (int i = 0; i < 4; i++) {
        free(tile->connections[i]);
    }
    free(tile);
}

void free_temp_dice(void* data) {
    temp_dice_t* dice = data;

    free(dice->identifier);
    for (int i = 0; i < 6; i++) {
        free(dice->dice[i]);
    }
    free(dice);
}

void free_name_mapping(game_data_t* game_data) {
    free_string_map(game_data->map->tile);
    free_string_map(game_data->map->connection);
    free_robin_hash(game_data->map->dice);
    free(game_data->map);
}

#endif
