#ifndef BOARD_H
#define BOARD_H

#include <stdlib.h>
#include "railroad_enums.h"
#include "railroad_types.h"

/************************************************************
*                                                           *
*   Structures keeping track of the game state.             *
*                                                           *
************************************************************/

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
 * The game board. Contains the following fields
 *
 *      expansions:     The expansions on the board.
 *      width:          The width of the board.
 *      height:         The height of the board.
 *      board:          A grid of tiles (width + 2) * (height + 2).
 */
typedef struct Board {
    /* Included expansions and which index they have */
    size_t expansion2index[TOTAL_EXPANSIONS];

    /* Board and information about it */
    board_size_t        height;
    board_size_t        width;
    Tile_t*             board;
} Board_t;

/**
 * A set of dice for the game board.
 *
 *      amount: The amount of dice in this dice bag.
 *      dice:   The dice in the dice bag.
 *
 */
typedef struct DiceBag {
    size_t amount;
    size_t* dice;
} DiceBag_t;

/**
 * Settings needed to randomize a game board.
 */
typedef struct RandomizeSettings {
} RandomizeSettings_t;

/*********************
*
* Data
*
**********************/

/**
 *
 */
typedef const struct ConnectionData {
    const bool traversable[TOTAL_CONNECTIONS];
    const bool non_connections[TOTAL_CONNECTIONS];
    const bool valid_connections[TOTAL_CONNECTIONS * TOTAL_CONNECTIONS];
    const bool networks[2 * 4 * TOTAL_TYPES];
} ConnectionData_t;

/**
 * Data for all the tiles.
 *
 *      expansion_index:    The index of the first tile for each expansion.
 *      tiles:              The tiles for each expansions, tile i has the
 *                          following information:
 *
 *                              tiles[i, 0] = tile_type
 *                              tiles[i, 1:5] = connections
 *                              tiles[i, 6] = stations
 */
typedef const struct TileData {
    /* Tiles */
    const size_t expansion_index[TOTAL_EXPANSIONS];
    const board_data_t tiles[6 * TOTAL_TILES];
} TileData_t;

/**
 *  Data for all the dice.
 *
 *      dice:   Dice data, dice i has the following information
 *
 *                  dice[i, 0] = expansion
 *                  dice[i, 1] = index in expansion
 *
 */
typedef const struct DiceData {
    const size_t dice[2 * 6 * TOTAL_DICE];
} DiceData_t;

/**
 * Contains all the mappings between names and integers
 */
typedef const struct GameMapping {
    void* expansions;
} GameMapping_t;

typedef const struct RailroadData {
    ConnectionData_t connection_data;
    TileData_t tile_data;
    DiceData_t dice_data;
    GameMapping_t game_mapping;
} RailroadData_t;

#endif

