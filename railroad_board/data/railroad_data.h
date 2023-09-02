#ifndef RAILROAD_DATA_H
#define RAILROAD_DATA_H

#include <stdlib.h>
#include "../railroad_constants.h"
#include "../railroad_types.h"
#include "../board/railroad_board.h"

#define LOAD_EXPANSION_FUNCTION(name, amount, ...) \
\
void load_##name##_tiles(GameData_t game_data, size_t index) { \
    const board_data_t tile_data[TILE_DATA_WIDTH * amount] = {__VA_ARGS__}; \
    for (size_t i = index; i < index + TILE_DATA_WIDTH * amount; i++) { \
        game_data.tile_data[i] = tile_data[i]; \
    } \
}

#define TILE_DATA_WIDTH     (6)

#define STANDARD_TILES      (17)
#define SPECIAL_TILES       (9)
#define MOUNTAIN_TILES      (3)
#define RIVER_TILES         (4)
#define LAKE_TILES          (6)
#define LAVA_TILES          (5)
#define METEOR_TILES        (9)

typedef struct GameData         GameData_t;
typedef struct ConnectionData   ConnectionData_t;
typedef struct TileData         TileData_t;

struct ConnectionData {
    const bool traversable[TOTAL_CONNECTIONS];
    const bool non_connections[TOTAL_CONNECTIONS];
    const bool valid_connections[TOTAL_CONNECTIONS * TOTAL_CONNECTIONS];
    const bool networks[DIRECTIONS * MAX_TILE_NETWORKS * TOTAL_TYPES];
};

struct GameData {
    const ConnectionData_t connection_data;
    board_data_t* tile_data;
};

/**
 * Returns a struct containing all the tile information and
 * connection information needed to play the game.
 */
const GameData_t load_expansions(size_t expansion_amount, ...);

/**
 * Loads a tile from a particular expansion.
 */
const Tile_t* load_tile(const GameData_t, size_t expansion_index, size_t local_index);

#endif

