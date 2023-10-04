#include <stdlib.h>
#include <stdio.h>

#include "../../utils/unittests.h"

#include "../railroad_types.h"
#include "../board/railroad_board.h"
#include "../board/AOS/board_struct.h"


board_load_info_t new_info(board_size_t height, board_size_t width) {
    board_load_info_t info;

    info.height = height;
    info.width = width;
    for (size_t i = 0; i < MAX_EXPANSIONS; i++)
        info.expansions[i] = NO_EXPANSION;

    return info;
}

UNITTEST(allocate_board) {
    board_t board;
    board_load_info_t info;

    info = new_info(1, 1);
    board = make_board(info);
    ASSERT_NOT_NULL(board, "No board was allocated.\n");

    free_board(board);
    END_UNITTEST;
}

UNITTEST(check_board_data) {
    board_t board;
    board_size_t height, width;
    board_load_info_t info;

    height = 2;
    width = 3;
    info = new_info(height, width);
    info.expansions[0] = e_STANDARD;
    info.expansions[1] = e_SPECIAL;

    board = make_board(info);
    ASSERT_NOT_NULL(board, "No board was allocated.");

    free_board(board);

    ASSERT_TRUE(board->height == height, "d");
    ASSERT_TRUE(board->width == width, "d");
    END_UNITTEST;
}

TEST(1,
    "allocate_board", allocate_board,
    "check_board_data", check_board_data
)

