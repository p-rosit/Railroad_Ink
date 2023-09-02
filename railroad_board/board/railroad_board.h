#ifndef BOARD_H
#define BOARD_H

#include "../railroad_constants.h"
#include "../railroad_types.h"

/**
 * The index of the tile where each field starts.
 */
enum TileDataIndex {
    EXPANSION_INDEX_INDEX   = 0,
    LOCAL_INDEX_INDEX       = 1,
    ORIENTATION_INDEX       = 2,
    CENTER_INDEX            = 3,
    DATA_INDEX              = 4,
    CONNECTIONS_INDEX       = 4 + MAX_EXPANSIONS,
    NETWORKS_INDEX          = 4 + MAX_EXPANSIONS + 4,
};

/**
 * A structure which holds information about a tile.
 */
typedef struct Tile {
    /* Expansion index and index in expansion */
    board_data_t expansion_index;
    board_data_t local_index;

    /* Info */
    board_data_t orientation;
    board_data_t center;
    board_data_t data[MAX_EXPANSIONS];

    /* Connection types and networks */
    board_data_t connections[4];
    board_data_t networks[8];
} Tile_t;

/**
 * The board which contains a set of tiles. The board must have a
 * width and a height.
 */
typedef struct Board *Board_t;

/**
 * Returns a board struct, the tiles will be allocated on the heap.
 */
Board_t make_board(board_size_t, board_size_t);

/**
 * Frees the allocated board.
 */
void free_board(Board_t);

/**
 * Returns true if the supplied set of coordinates exists on the board.
 */
bool coord_on_board(Board_t, size_t, size_t);

/**
 * Add tile to board in position i, j.
 */
void add_tile_to_board(Board_t, Tile_t, size_t, size_t);

/**
 * Sets the data at some index of a tile at some coordinate to some value
 */
void set_tile_data(Board_t, board_data_t, size_t, size_t, size_t);

/**
 * Rudimentary print of a board. Prints a specific layer.
 */
void print_board(Board_t, size_t);

#endif

