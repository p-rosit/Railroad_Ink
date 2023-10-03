#include "../../../cut/cut.h"

#include "../../railroad_types.h"
#include "../railroad_board.h"


board_coord_t make_coord(board_size_t row, board_size_t col) {
    return (board_coord_t) {.row = row, .col = col};
}


board_load_info_t new_info(board_size_t height, board_size_t width) {
    board_load_info_t info;

    info.height = height;
    info.width = width;
    for (size_t i = 0; i < MAX_EXPANSIONS; i++)
        info.expansions[i] = NO_EXPANSION;

    return info;
}

UNIT_TEST(init_board) {
    board_t board;
    board_load_info_t info;

    info = new_info(1, 1);
    board = make_board(info);
    ASSERT_NOT_NULL(board, "No board was allocated.");

    TEST_END;
}

UNIT_TEST(test_coord_on_board) {
    size_t i, j;
    board_t board;
    board_load_info_t info;
    board_size_t height, width;
    bool exists;

    height = 3;
    width = 7;

    info = new_info(height, width);
    board = make_board(info);

    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {
            exists = coord_on_board(board, make_coord(i + 1, j + 1));
            ASSERT_TRUE(exists, "Coordinate (%d, %d) does not exist on board of size (%d, %d).", i+1, j+1, height, width);
        }
    }

    for (i = 0; i < height + 2; i++) {
        exists = coord_on_board(board, make_coord(i, 0));
        ASSERT_FALSE(exists, "Coordinate (%d, 0) should not exist on board of size (%d, %d).", i, height, width);

        exists = coord_on_board(board, make_coord(i, width + 1));
        ASSERT_FALSE(exists, "Coordinate (%d, %d) should not exist on board of size (%d, %d).", i, width + 1, height, width);
    }

    for (j = 0; j < width + 2; j++) {
        exists = coord_on_board(board, make_coord(0, j));
        ASSERT_FALSE(exists, "Coordinate (0, %d) should not exist on board of size (%d, %d).", j, height, width);

        exists = coord_on_board(board, make_coord(height + 1, j));
        ASSERT_FALSE(exists, "Coordinate (%d, %d) should not exist on board of size (%d, %d).", height + 1, j, height, width);
    }

    TEST_END;
}


LIST_TESTS(
    init_board,
    test_coord_on_board
)

