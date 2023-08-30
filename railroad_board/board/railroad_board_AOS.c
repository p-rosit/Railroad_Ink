#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../railroad_constants.h"
#include "../railroad_types.h"
#include "../railroad_structs.h"

/* Functions and structs which are implemented */
#include "../railroad_board.h"

struct Tile {
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
};

/**
 * The game board. Contains the following fields
 *
 *      expansions:     The expansions on the board.
 *      width:          The width of the board.
 *      height:         The height of the board.
 *      board:          A grid of tiles (width + 2) * (height + 2).
 */
struct Board {
    /* Included expansions and which index they have */
    size_t expansions[MAX_EXPANSIONS];

    /* Board and information about it */
    board_size_t        height;
    board_size_t        width;
    Tile_t*             board;
};


Board_t make_board(board_size_t height, board_size_t width) {
    size_t board_depth = 4 + MAX_EXPANSIONS + 12;
    size_t total_entries;
    Tile_t* board;

    total_entries = (height + 2) * (width * 2);
    board = calloc(total_entries, sizeof(Tile_t));

    return (Board_t) {
        .height = height,
        .width = width,
        .board = board,
    };
}

void free_board(Board_t board) {
    free(board.board);
}

bool coord_on_board(Board_t board, size_t i, size_t j) {
    return (1 <= i) && (i <= board.height) && (1 <= j) && (j <= board.width);
}

void add_tile_to_board(Board_t board, Tile_t tile, size_t i, size_t j) {
    memcpy(&board.board[i * board.width + j], &tile, sizeof(Tile_t));
}

void print_board(Board_t board) {
    board_size_t i, j;

    for (i = 0; i < board.height; i++) {
        for (j = 0; j < board.width; j++) {
            printf("%lu ", board.board[i * board.width + j].local_index);
        }
        printf("\n");
    }
}

