#define DEBUG

#include "data_utils.h"
#include "load_data.h"
#include "../utils/hash_map.h"
#include "../utils/debug_utils.h"

int main() {
    uint8_t amount;
    char** expansion_name;
    game_data_t* data;
    tile_data_t* tiles;
    hash_map_t* map;

    data = malloc(sizeof(game_data_t));

    load_info(&amount, &expansion_name, &map);

    tiles = load_tiles(amount, expansion_name);
    data->tiles = tiles;

    printf("Expansions (%d): (%s", amount, expansion_name[0]);
    for (int i = 1; i < amount; ++i) {
        printf(", %s", expansion_name[i]);
    }
    printf(")\n");
    print_hash_map(map);

    free(data);
    free(tiles);
    free_hash_map(map);
    for (int i = 0; i < amount; ++i) free(expansion_name[i]);
    free(expansion_name);

    return 0;
}
