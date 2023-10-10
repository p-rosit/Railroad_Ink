#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "../../utils/railroad_constants.h"
#include "../../utils/railroad_types.h"
#include "../../data/railroad_data.h"
#include "board_struct.h"

/* Functions and structs which are implemented */
#include "../railroad_board.h"


bool coord_on_board(board_t board, board_coord_t coord) {
    bool in_cols = (1 <= coord.row) && (coord.row <= board->height);
    bool in_rows = (1 <= coord.col) && (coord.col <= board->width);
    return in_cols && in_rows;
}

void add_tile_to_board(board_t board, tile_t tile, board_coord_t coord) {
    memcpy(&board->tiles[coord.row * (board->width + 2) + coord.col], &tile, sizeof(tile_t));
}

void set_tile_data(board_t board, board_data_t value, size_t index, board_coord_t coord) {
    board->tiles[coord.row * (board->width + 2) + coord.col].data[index] = value;
}

expansion_index_t* get_board_expansions(board_t board) {
    return board->expansions;
}

