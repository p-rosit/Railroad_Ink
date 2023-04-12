#ifndef LOAD_GAME_DATA_H
#define LOAD_GAME_DATA_H

#include "data_utils.h"
#include "load_expansions.h"
#include "load_tile_info.h"
#include "load_tile_data.h"
#include "generate_type_data.h"
#include "../utils/hash_map.h"
#include "../utils/debug_utils.h"

game_data_t* load_game_data(char*);
game_data_t* allocate_game_data();
void         write_game_data();

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

    free(types_in_expansions);
    for (int i = 0; i < expansion_amount; i++) free(expansion_names[i]);
    free(expansion_names);

    free(tile_info);
    free(tile_data);
    free(type_data);

    free_hash_map(expansion2int);
    free_hash_map(tile_id2int);
    free_hash_map(connection2int);
    free_hash_map(type2int);

    return NULL;
}

game_data_t* allocate_game_data() {
    return NULL;
}

void write_game_data() {
    return;
}

#endif
