#ifndef BOARD_H
#define BOARD_H

#include <stdlib.h>
#include "railroad_types.h"

/************************************************************
*                                                           *
*   Structures keeping track of the game state.             *
*                                                           *
************************************************************/

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
 *
 */
typedef struct Board {
    board_size_t width;
    board_size_t height;
    board_data_t* board;
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

#endif

