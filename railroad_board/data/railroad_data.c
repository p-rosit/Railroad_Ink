#include <stdio.h>
#include "../railroad_types.h"
#include "../railroad_expansions.h"
#include "../railroad_constants.h"
#include "data_utils.h"
#include "expansion_data.c"
#include "railroad_data.h"

size_t count_expansion_tiles(expansion_index_t);
size_t load_expansion(board_data_t*, expansion_index_t, size_t);


const game_data_t load_data(expansion_index_t expansions[MAX_EXPANSIONS]) {
    size_t i, tile_amount, index, expansion_index[MAX_EXPANSIONS];
    board_data_t* tile_data;

    tile_amount = 0;
    for (i = 0; i < MAX_EXPANSIONS; i++) {
        if (expansion_has_tiles(expansions[i])) {
            tile_amount += count_expansion_tiles(expansions[i]);
        }
    }

    index = 0;
    tile_data = malloc(TILE_DATA_WIDTH * tile_amount * sizeof(board_data_t));
    for (i = 0; i < MAX_EXPANSIONS; i++) {
        expansion_index[i] = index;
        index = load_expansion(tile_data, expansions[i], index);
    }

    game_data_t game_data = (game_data_t ) {
        .connection_data = (connection_data_t) {
            .traversable = TRAVERSABLE_DATA,
            .non_connections = NON_CONNECTION_DATA,
            .valid_connections = VALID_CONNECTIONS_DATA,
            .networks = CONNECTION_DATA,
        },
        .tile_data = tile_data,
    };

    for (i = 0; i < MAX_EXPANSIONS; i++) {
        ((expansion_index_t*) game_data.expansion_index)[i] = expansion_index[i];
    }

    return game_data;
}

void free_data(const game_data_t game_data) {
    free((board_data_t*) game_data.tile_data);
}

tile_t load_tile(const game_data_t game_data, tile_data_t index) {
    size_t i;
    tile_t tile;

    tile.expansion_index = index.expansion_index;
    tile.local_index = index.local_index;
    tile.orientation = 0; /* TODO */
    tile.center = 0; /* TODO */
    for (i = 0; i < MAX_TYPE_EXPANSIONS; i++) {
        tile.data[i] = 0; /* TODO*/
    }
    for (i = 0; i < DIRECTIONS; i++) {
        tile.connections[i] = 0; /* TODO */
    }
    for (i = 0; i < MAX_TILE_NETWORKS * DIRECTIONS; i++) {
        tile.networks[i] = 0; /* TODO */
    }

    return tile;
}

size_t load_expansion(board_data_t* tile_data, expansion_index_t expansion, size_t index) {
    switch (expansion) {
        case e_STANDARD:
            return load_standard_tiles(tile_data, index);
        case e_SPECIAL:
            return load_special_tiles(tile_data, index);
        case e_MOUNTAIN:
            return load_mountain_tiles(tile_data, index);

        case e_RIVER:
            return load_river_tiles(tile_data, index);
        case e_LAKE:
            return load_lake_tiles(tile_data, index);

        case e_LAVA:
            return load_lava_tiles(tile_data, index);
        case e_METEOR:
            return load_meteor_tiles(tile_data, index);

        case NO_EXPANSION:
            return index;
        default:
            printf("WARNING: Trying to load unknown expansion with index %lu.\n", expansion);
            return index;
    }
}

size_t count_expansion_tiles(expansion_index_t expansion) {
    switch (expansion) {
        case e_STANDARD:
            return STANDARD_TILES;
        case e_SPECIAL:
            return SPECIAL_TILES;
        case e_MOUNTAIN:
            return MOUNTAIN_TILES;

        case e_RIVER:
            return RIVER_TILES;
        case e_LAKE:
            return LAKE_TILES;

        case e_LAVA:
            return LAVA_TILES; 
        case e_METEOR:
            return METEOR_TILES;

        case NO_EXPANSION:
            return 0;
        default:
            printf("WARNING: Trying to count tiles of unknown expansion with index %lu.\n", expansion);
            return 0;
    }
}

