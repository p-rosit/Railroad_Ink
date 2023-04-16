#ifndef LOAD_GAME_DATA_H
#define LOAD_GAME_DATA_H

#include "data_utils.h"
#include "load_expansions.h"
#include "load_tile_info.h"
#include "load_tile_data.h"
#include "generate_type_data.h"
#include "../utils/debug_utils.h"

game_data_t* load_game_data(char*);
game_data_t* repack_game_data(tile_info_t*, tile_data_t*, type_data_t*, tile_textures_t*);
void         free_game_data(game_data_t*);

game_data_t* load_game_data(char* directory_name) {
    uint8_t expansion_amount, max_combinations, *types_in_expansions;
    uint16_t type_amount;
    char** expansion_names;
    hash_map_t *expansion2int, *tile_id2int, *connection2int, *type2int;
    game_data_t* game_data;
    tile_info_t* tile_info;
    tile_data_t* tile_data;
    type_data_t* type_data;

    expansion_amount = 0;
    load_info(directory_name, &expansion_amount, &expansion_names, &expansion2int, &max_combinations);
    tile_info = load_tile_info(directory_name, &tile_id2int, &connection2int);

    type_amount = 0;
    tile_data = load_tiles(directory_name, expansion_amount, expansion_names, tile_id2int, connection2int, &type_amount, &type2int, &types_in_expansions);

    type_data = generate_type_data(max_combinations, expansion_amount, types_in_expansions);

    //repack data
    game_data = repack_game_data(tile_info, tile_data, type_data, NULL);

    free(types_in_expansions);
    for (int i = 0; i < expansion_amount; i++) free(expansion_names[i]);
    free(expansion_names);

    free_tile_info(tile_info);
    free_tile_data(tile_data);
    free_type_data(type_data);

    free_hash_map(expansion2int);
    free_hash_map(tile_id2int);
    free_hash_map(connection2int);
    free_hash_map(type2int);

    return game_data;
}

game_data_t* repack_game_data(tile_info_t* info, tile_data_t* tiles, type_data_t* types, tile_textures_t* textures) {
    int i;
    size_t bool_size, u8_size, u16_size;
    bool* bool_ptr;
    uint8_t* u8_ptr;
    uint16_t* u16_ptr;
    game_data_t* game_data;

    game_data = (game_data_t*) malloc(sizeof(game_data_t));
    game_data->info = (tile_info_t*) malloc(sizeof(tile_info_t));
    game_data->tiles = (tile_data_t*) malloc(sizeof(tile_data_t));
    game_data->types = (type_data_t*) malloc(sizeof(type_data_t));
    game_data->textures = textures;

    bool_size = 8 * info->id_amount;
    bool_size += 2 * info->connection_amount;
    bool_size += info->connection_amount * info->connection_amount;

    u8_size = 7 * tiles->tile_amount;
    for (i = 0; i < types->max_combinations; i++) {
        u8_size += (i + 1) * types->combination_ind[i];
    }

    u16_size = tiles->expansion_amount;
    u16_size += 2 * types->max_combinations;
    u16_size += types->hash_size;

    bool_ptr = (bool*) malloc(bool_size * sizeof(bool));
    u8_ptr = (uint8_t*) malloc(u8_size * sizeof(uint8_t));
    u16_ptr = (uint16_t*) malloc(u16_size * sizeof(uint16_t));

    // Allocate tile info
    game_data->info->networks = bool_ptr;
    bool_ptr += 8 * info->id_amount;

    game_data->info->traversable = bool_ptr;
    bool_ptr += info->connection_amount;

    game_data->info->non_connection = bool_ptr;
    bool_ptr += info->connection_amount;

    game_data->info->valid_connection = bool_ptr;
    bool_ptr += info->connection_amount * info->connection_amount;

    // Allocate tile data
    game_data->tiles->expansion_amount = tiles->expansion_amount;
    game_data->tiles->tile_amount = tiles->tile_amount;

    game_data->tiles->expansion_ind = u16_ptr;
    u16_ptr += tiles->expansion_amount;

    game_data->tiles->ids = u8_ptr;
    u8_ptr += tiles->tile_amount;

    game_data->tiles->types = u8_ptr;
    u8_ptr += tiles->tile_amount;

    game_data->tiles->connections = u8_ptr;
    u8_ptr += 4 * tiles->tile_amount;

    game_data->tiles->stations = u8_ptr;
    u8_ptr += tiles->tile_amount;

    // Allocate type data
    game_data->types->combination_ind = u16_ptr;
    u16_ptr += types->max_combinations;

    game_data->types->start_index = u16_ptr;
    u16_ptr += types->max_combinations;

    game_data->types->index2types = u8_ptr;
    u8_ptr += types->start_index[types->max_combinations - 1];

    game_data->types->types2index = u16_ptr;
    u16_ptr += types->hash_size;

    // Write to tile info 
    game_data->info->connection_amount = info->connection_amount;
    game_data->info->id_amount = info->id_amount;

    for (i = 0; i < 8 * info->id_amount; i++) {
        game_data->info->networks[i] = info->networks[i];
    }
    for (i = 0; i < info->connection_amount; i++) {
        game_data->info->traversable[i] = info->traversable[i];
        game_data->info->non_connection[i] = info->non_connection[i];
    }
    for (i = 0; i < info->connection_amount * info->connection_amount; i++) {
        game_data->info->valid_connection[i] = info->valid_connection[i];
    }

    // Write to tile data
    game_data->tiles->expansion_amount = tiles->expansion_amount;
    game_data->tiles->tile_amount = tiles->tile_amount;

    for (i = 0; i < tiles->tile_amount; i++) {
        game_data->tiles->ids[i] = tiles->ids[i];
        game_data->tiles->types[i] = tiles->types[i];
        game_data->tiles->stations[i] = tiles->stations[i];
    }
    for (i = 0; i < 4 * tiles->tile_amount; i++) {
        game_data->tiles->connections[i] = tiles->connections[i];
    }

    // Write to type data
    game_data->types->max_combinations = types->max_combinations;
    game_data->types->hash_size = types->hash_size;

    for (i = 0; i < types->max_combinations; i++) {
        game_data->types->combination_ind[i] = types->combination_ind[i];
        game_data->types->start_index[i] = types->start_index[i];
    }
    for (i = 0; i < types->start_index[types->max_combinations - 1]; i++) {
        game_data->types->index2types[i] = types->index2types[i];
    }
    for (i = 0; i < types->hash_size; i++) {
        game_data->types->types2index[i] = types->types2index[i];
    }

    return game_data;
}

void free_game_data(game_data_t* game_data) {
    if (game_data == NULL) return;
    
    free(game_data->info->networks);
    free(game_data->tiles->expansion_ind);
    free(game_data->tiles->ids);

    free(game_data->info);
    free(game_data->tiles);
    free(game_data->types);
    free(game_data->textures);
    free(game_data);
}

#endif
