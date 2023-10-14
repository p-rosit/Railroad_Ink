#include <string.h>
#include "../../../cut/cut.h"

#include "../../utils/railroad_types.h"
#include "../railroad_board.h"


void copy_expansions(expansion_index_t dst[TOTAL_EXPANSIONS], const expansion_index_t src[TOTAL_EXPANSIONS]) {
    memcpy(dst, src, MAX_EXPANSIONS * sizeof(expansion_index_t));
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
    bool exists = false;

    for (size_t i = 0; i < MAX_EXPANSIONS; i++) {
        if (expansions[i] == expansion) {
            exists = true;
            break;
        }
    }

    return exists;
}

bool has_duplicates(expansion_index_t expansions[MAX_EXPANSIONS], expansion_index_t expansion) {
    int count;

    for (size_t i = 0, count = 0; i < MAX_EXPANSIONS; i++) {
        if (expansions[i] == expansion) {
            count += 1;
            if (count > 1) {
                return true;
            }
        }
    }

    return false;
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
    size_t i, index1, index2;
    bool in_info, in_board;
    board_t board;
    expansion_index_t exp1, exp2, expansions[MAX_EXPANSIONS];
    board_load_info_t info;

    for (exp1 = e_STANDARD; exp1 < TOTAL_EXPANSIONS; exp1++) {
        for (exp2 = e_STANDARD; exp2 < TOTAL_EXPANSIONS; exp2++) {
            if (exp1 == exp2) continue;
            for (index1 = 0; index1 < MAX_EXPANSIONS; index1++) {
                for (index2 = 0; index2 < MAX_EXPANSIONS; index2++) {
                    if (index1 == index2) continue;

                    clear_expansion_list(expansions);
                    expansions[index1] = exp1;
                    expansions[index2] = exp2;

                    info = new_info(1, 1);
                    copy_expansions(info.expansions, expansions);
                    board = make_board(info);

                    for (i = 0; i < TOTAL_EXPANSIONS; i++) {
                        in_info = expansion_exists(expansions, i);
                        in_board = expansion_exists(get_board_expansions(board), i);

                        if (in_info) {
                            ASSERT_EQUAL(in_info, in_board, "{%lu: %lu, %lu: %lu}: Expansion %lu was not loaded.", index1, exp1, index2, exp2, i);
                        } else {
                            ASSERT_EQUAL(in_info, in_board, "{%lu: %lu, %lu: %lu}: Expansion %lu should not have been loaded.", index1, exp1, index2, exp2, i);
                        }

                        if (in_board) {
                            ASSERT_FALSE(
                                has_duplicates(get_board_expansions(board), i),
                                "{%lu: %lu, %lu: %lu}: Expansion %lu was loaded several times.", index1, exp1, index2, exp2, i
                            );
                        }
                    }
                }
            }
        }
    }

    TEST_END;
}


LIST_TESTS(
    test_coord_on_board,
    get_expansions
)

