#ifndef BOARD_STRUCT_H
#define BOARD_STRUCT_H
#include "../../utils/railroad_types.h"
#include "../../utils/railroad_expansions.h"
#include "../../utils/railroad_constants.h"
#include "../../data/railroad_data.h"

/**
 * The game board. Contains the following fields
 *
 *      expansions:     The expansions on the board.
 *      width:          The width of the board.
 *      height:         The height of the board.
 *      board:          A grid of tiles (width + 2) * (height + 2).
 */
struct board {
    /* Included expansions and which index they have */
    size_t expansions[MAX_EXPANSIONS];
    size_t tile_expansions[MAX_EXPANSIONS];
    size_t type_expansions[MAX_EXPANSIONS];

    /* Game data */
    const game_data_t game_data;

    /* Board and information about it */
    board_size_t        height;
    board_size_t        width;
    tile_t*             tiles;
};

#endif

