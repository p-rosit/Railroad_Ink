#define DEBUG_LEVEL (0)

#include "data_utils.h"
#include "load_expansions.h"
#include "load_tile_info.h"
#include "load_tile_data.h"
#include "../utils/hash_map.h"
#include "../utils/debug_utils.h"

int main() {
    uint8_t amount;
    uint16_t type_amount;
    char** expansion_name;
    game_data_t* data;
    tile_info_t* tile_info;
    tile_data_t* tiles;
    hash_map_t *map, *tile_ids, *connections, *types;

    data = malloc(sizeof(game_data_t));

    amount = 0;
    load_info(&amount, &expansion_name, &map);

    tile_info = load_tile_info(&tile_ids, &connections);

    type_amount = 0;
    tiles = load_tiles(amount, expansion_name, tile_ids, connections, &type_amount, &types);
//    data->tiles = tiles;

    free(data);
    free(tiles);
    free(tile_info);
    free_hash_map(map);
    free_hash_map(tile_ids);
    free_hash_map(connections);
    free_hash_map(types);
    for (int i = 0; i < amount; ++i) free(expansion_name[i]);
    free(expansion_name);

    return 0;
}
