#include "../../../cut/cut.h"

#include "../../utils/railroad_types.h"
#include "../railroad_board.h"


board_load_info_t new_info(board_size_t height, board_size_t width) {
    board_load_info_t info;

    info.height = height;
    info.width = width;
    for (size_t i = 0; i < MAX_EXPANSIONS; i++) {
        info.expansions[i] = NO_EXPANSION;
    }

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

UNIT_TEST(too_many_type_expansions) {
    board_t board;
    board_load_info_t info;

    info = new_info(1, 1);
    info.expansions[0] = e_METEOR;
    info.expansions[1] = e_METEOR;
    info.expansions[2] = e_BUILDING;
    info.expansions[3] = e_BUILDING;
    info.expansions[4] = e_BUILDING;

    board = make_board(info);
    ASSERT_NULL(board, "Too many type expansions were allowed to load.");

    TEST_END;
}

UNIT_TEST(init_normal) {
    board_t board;
    board_load_info_t info;

    info = new_info(1, 1);
    info.expansions[0] = e_STANDARD;
    info.expansions[1] = e_SPECIAL;
    info.expansions[2] = e_NETWORK;
    info.expansions[3] = e_LONGEST;
    info.expansions[4] = e_ERROR;

    board = make_board(info);
    ASSERT_NOT_NULL(board, "Board with normal expansions could not be loaded");

    TEST_END;
}

UNIT_TEST(init_with_buildings) {
    board_t board;
    board_load_info_t info;

    info = new_info(1, 1);
    info.expansions[0] = e_STANDARD;
    info.expansions[1] = e_SPECIAL;
    info.expansions[2] = e_NETWORK;
    info.expansions[3] = e_LONGEST;
    info.expansions[4] = e_ERROR;
    info.expansions[5] = e_BUILDING;

    board = make_board(info);
    ASSERT_NOT_NULL(board, "Board with normal expansions and buildings could not be loaded");

    TEST_END;
}

UNIT_TEST(init_with_invalid_expansion) {
    board_t board;
    board_load_info_t info;

    info = new_info(1, 1);
    info.expansions[0] = TOTAL_EXPANSIONS;

    board = make_board(info);
    ASSERT_NULL(board, "Board was allowed to load with an invalid expansion: %d.", TOTAL_EXPANSIONS);

    TEST_END;
}

UNIT_TEST(test_free_board) {
    board_t board;
    board_load_info_t info;

    info = new_info(1, 1);
    info.expansions[0] = e_STANDARD;

    board = make_board(info);
    ASSERT_NOT_NULL(board, "Previous test failed.");
    free_board(board);

    TEST_END;
}

UNIT_TEST(test_free_no_board) {
    free_board(NULL);
    TEST_END;
}

LIST_TESTS(
    init_board,
    too_many_type_expansions,
    init_normal,
    init_with_buildings,
    init_with_invalid_expansion,
    test_free_board,
    test_free_no_board,
)

