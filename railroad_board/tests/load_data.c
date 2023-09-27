#include <stdio.h>
#include "../board/railroad_board.h"
#include "../data/railroad_data.h"

int main() {
    tile_t tile;
    board_t board;
    board_load_info_t info;

    info.height = 5;
    info.width = 9;
    for (size_t i = 0; i < MAX_EXPANSIONS; i++) {
        info.expansions[i] = NO_EXPANSION;
    }
    info.expansions[0] = e_STANDARD;
    info.expansions[1] = e_SPECIAL;

    board = make_board(info);

    print_board(board, 0);
    print_game_data(board);
    free_board(board);
    return 0;
}
