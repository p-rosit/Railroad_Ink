#include <stdlib.h>
#include "railroad_enums.h"
#include "railroad_types.h"
#include "railroad_structs.h"


Board_t make_board(board_size_t height, board_size_t width, Tile_t* board) {
    size_t board_depth = 4;
    size_t total_entries;

    total_entries = (height + 2) * (width * 2) * board_depth;

    return (Board_t) {
        .height = height,
        .width = width,
        .board = board,
    };
}

void set_tile(
    RailroadData_t data,
    Board_t board,
    size_t i,
    size_t j,
    board_data_t expansion_index,
    board_data_t index,
    board_data_t orientation
) {
    
}

void load_default_board(RailroadData_t data, Board_t board) {
    board_data_t* t;

    set_tile(data, board, 0, 2, 1, 7, 2);
}

