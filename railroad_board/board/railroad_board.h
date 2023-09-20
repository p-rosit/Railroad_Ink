#ifndef BOARD_H
#define BOARD_H

#include "../railroad_constants.h"
#include "../railroad_types.h"
#include "../railroad_expansions.h"
#include "../data/railroad_data.h"
#include "railroad_board_structs.h"

typedef struct board *board_t;

/**
 * The index of the tile where each field starts.
 */
enum TileDataIndex {
    EXPANSION_INDEX_INDEX   = 0,
    LOCAL_INDEX_INDEX       = 1,
    ORIENTATION_INDEX       = 2,
    CENTER_INDEX            = 3,
    DATA_INDEX              = 4,
    CONNECTIONS_INDEX       = 4 + MAX_TYPE_EXPANSIONS,
    NETWORKS_INDEX          = 4 + MAX_TYPE_EXPANSIONS + 4,
};


void print_game_data(board_t);

/**
 * Returns a board struct, the tiles will be allocated on the heap.
 */
board_t make_board(board_load_info_t);

/**
 * Frees the allocated board.
 */
void free_board(board_t);

/**
 * Returns true if the supplied set of coordinates exists on the board.
 */
bool coord_on_board(board_t, board_coord_t);

/**
 * Add tile to board in position i, j.
 */
void add_tile_to_board(board_t, tile_t, board_coord_t);

/**
 * Sets the data at some index of a tile at some coordinate to some value
 */
void set_tile_data(board_t, board_data_t, size_t, board_coord_t);

/**
 * Rudimentary print of a board. Prints a specific layer.
 */
void print_board(board_t, size_t);

#endif

