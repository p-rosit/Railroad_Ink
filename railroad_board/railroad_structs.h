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
    board_data_t expansion_index;
    board_data_t index;
    board_data_t type;
    board_data_t orientation;
} Tile_t;

/**
 * The game board. Contains the following fields
 *
 *      width:  The width of the board
 *      height: The height of the board
 *      board:  The data in the board, tile (i, j) is
 *              saved in the following way
 *
 *                  board[i, j, 0] = expansion
 *                  board[i, j, 1] = index
 *                  board[i, j, 2] = type
 *                  board[i, j, 3] = orientation
 *                  board[i, j, 4:8] = connections
 *                  board[i, j, 8:16] = network
 *
 */
typedef struct Board {
    board_size_t height;
    board_size_t width;
    Tile_t* board;
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

/* Version */
#define DATA_VERSION            (1)

/* Settings */
#define MAX_COMBINATIONS        (4)

/* Tile information */
#define TOTAL_TILES             (53)
#define TOTAL_DICE              (5)

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

