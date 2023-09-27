#ifndef RAILROAD_DATA_H
#define RAILROAD_DATA_H

#define AOS
#include <stdlib.h>
#include "../railroad_constants.h"
#include "../railroad_expansions.h"
#include "../railroad_types.h"
#include "../board/railroad_board_structs.h"


#define STANDARD_TILES      (17)
#define SPECIAL_TILES       (9)
#define MOUNTAIN_TILES      (3)
#define RIVER_TILES         (4)
#define LAKE_TILES          (6)
#define LAVA_TILES          (5)
#define METEOR_TILES        (9)

typedef struct connection_data {
    const bool traversable[TOTAL_CONNECTIONS];
    const bool non_connections[TOTAL_CONNECTIONS];
    const bool valid_connections[TOTAL_CONNECTIONS * TOTAL_CONNECTIONS];
    const bool networks[DIRECTIONS * MAX_TILE_NETWORKS * TOTAL_TYPES];
} connection_data_t;

typedef struct game_data {
    const connection_data_t connection_data;
    const size_t expansion_index[MAX_EXPANSIONS];
    const board_data_t* tile_data;
} game_data_t;

/**
 * Load data for a game board.
 */
const game_data_t load_data(expansion_index_t expansions[MAX_EXPANSIONS]);

/**
 * Free game data.
 */
void free_data(const game_data_t);

/**
 * Loads a tile from a particular expansion.
 */
tile_t load_tile(const game_data_t, tile_data_t);

#endif

