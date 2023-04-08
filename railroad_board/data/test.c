#define DEBUG_LEVEL (0)

#include "data_utils.h"
#include "load_data.h"
#include "../utils/hash_map.h"
#include "../utils/debug_utils.h"

int main() {
    uint8_t amount;
    char** expansion_name;
    game_data_t* data;
    tile_info_t* tile_info;
    tile_data_t* tiles;
    hash_map_t *map, *tile_ids, *connections;

    data = malloc(sizeof(game_data_t));

    amount = 0;
    load_info(&amount, &expansion_name, &map);

    tile_info = load_tile_info(&tile_ids, &connections);

    tiles = load_tiles(amount, expansion_name, tile_ids, connections);
//    data->tiles = tiles;

    printf("Expansions (%d): (%s", amount, expansion_name[0]);
    for (int i = 1; i < amount; ++i) {
        printf(", %s", expansion_name[i]);
    }
    printf(")\n\n");
    //print_hash_map(map);
    free(data);
//    free(tiles);
    free(tile_info);
    free_hash_map(map);
    free_hash_map(tile_ids);
    free_hash_map(connections);
    for (int i = 0; i < amount; ++i) free(expansion_name[i]);
    free(expansion_name);

    return 0;
}
