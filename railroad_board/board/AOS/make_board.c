#include <stdio.h>
#include <stdarg.h>

#include "../railroad_board.h"
#include "board_struct.h"

board_t make_board(board_load_info_t info) {
    size_t total_entries;
    tile_t* tiles;
    board_t board;

    total_entries = (info.height + 2) * (info.width * 2);

    board = malloc(sizeof(struct board));
    board->width = info.width;
    board->height = info.height;
    board->tiles = calloc(total_entries, sizeof(tile_t));

    return board;
}

board_t load_board_data(board_t board, board_load_info_t info) {
    /* Bypass C type system to assign to const once */
    GameData_t game_data = load_data(board->expansions);
    memcpy((struct GameData *) &(board->game_data), &game_data, sizeof(GameData_t));
}
