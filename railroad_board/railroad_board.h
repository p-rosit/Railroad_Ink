#ifndef BOARD_H
#define BOARD_H

#include "railroad_constants.h"
#include "railroad_types.h"

/**
 * A structure which holds information about a tile.
 */
typedef struct Tile Tile_t;

/**
 * The board which contains a set of tiles. The board must have a
 * width and a height.
 */
typedef struct Board Board_t;

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
 * Rudimentary print of a board.
 */
void print_board(Board_t);

#endif

