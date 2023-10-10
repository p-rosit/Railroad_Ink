#include <string.h>
#include "../../../cut/cut.h"

#include "../../utils/railroad_types.h"
#include "../railroad_board.h"


void copy_expansions(expansion_index_t dst[TOTAL_EXPANSIONS], const expansion_index_t src[TOTAL_EXPANSIONS]) {
    memcpy(dst, src, TOTAL_EXPANSIONS * sizeof(expansion_index_t));
}


board_coord_t make_coord(board_size_t row, board_size_t col) {
    return (board_coord_t) {.row = row, .col = col};
}


void clear_expansion_list(expansion_index_t expansions[MAX_EXPANSIONS]) {
    for (size_t i = 0; i < MAX_EXPANSIONS; i++) {
        expansions[i] = NO_EXPANSION;
    }
}


board_load_info_t new_info(board_size_t height, board_size_t width) {
    board_load_info_t info;

    info.height = height;
    info.width = width;
    clear_expansion_list(info.expansions);

    return info;
}

bool expansion_exists(expansion_index_t expansions[MAX_EXPANSIONS], expansion_index_t expansion) {
    bool exists;

    for (size_t i = 0, exists = false; i < MAX_EXPANSIONS; i++) {
        if (expansions[i] == expansion) {
            exists = true;
            break;
        }
    }

    return exists;
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


UNIT_TEST(get_expansions) {
    size_t i;
    bool in_info, in_board;
    board_t board;
    expansion_index_t expansions[MAX_EXPANSIONS];
    board_load_info_t info;

    clear_expansion_list(expansions);
    expansions[0] = e_SPECIAL;
    expansions[1] = e_DESERT;
    expansions[4] = e_ERROR;

    info = new_info(1, 1);
    copy_expansions(info.expansions, expansions);

    board = make_board(info);

    for (i = 0; i < TOTAL_EXPANSIONS; i++) {
        in_info = expansion_exists(expansions, i);
        in_board = expansion_exists(get_board_expansions(board), i);

        if (in_info) {
            ASSERT_EQUAL(in_info, in_board, "Expansion %lu was not loaded.", i);
        } else {
            ASSERT_EQUAL(in_info, in_board, "Expansion %lu should not have been loaded.", i);
        }
    }

    TEST_END;
}


LIST_TESTS(
    test_coord_on_board,
    get_expansions
)

