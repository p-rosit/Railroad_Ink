#ifndef RAILROAD_BOARD_STRUCTS_H
#define RAILROAD_BOARD_STRUCTS_H

#include "../utils/railroad_types.h"
#include "../utils/railroad_expansions.h"
#include "../utils/railroad_constants.h"

/**
 * A structure which holds information about a tile.
 */
typedef struct tile {
    /* Expansion index and index in expansion */
    board_data_t expansion_index;
    board_data_t local_index;

    /* Info */
    board_data_t orientation;
    board_data_t center;
    board_data_t data[MAX_TYPE_EXPANSIONS];

    /* Connection types and networks */
    board_data_t connections[DIRECTIONS];
    board_data_t networks[MAX_TILE_NETWORKS * DIRECTIONS];
} tile_t;

typedef struct board_coord {
    board_size_t row;
    board_size_t col;
} board_coord_t;

typedef struct board_shape {
    board_size_t height;
    board_size_t width;
} board_shape_t;

typedef struct tile_load_data {
    size_t global_expansion_index;
    size_t local_expansion_index;
    size_t local_index;
} tile_load_data_t;

typedef struct board_load_info {
    board_shape_t shape;
    expansion_index_t expansions[MAX_EXPANSIONS];
} board_load_info_t;

#endif

