#include <stdio.h>
#include "../board/railroad_board.h"
#include "../data/railroad_data.h"

int main() {
    size_t i;
    Board_t board;

    board = make_board(4, 5, 2, e_STANDARD, e_SPECIAL);

    print_board(board, 0);
    print_game_data(board);
    free_board(board);
    return 0;
}
