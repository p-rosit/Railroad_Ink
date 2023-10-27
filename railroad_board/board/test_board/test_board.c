#include <string.h>
#include "../../../cut/cut.h"

#include "../../utils/railroad_types.h"
#include "../railroad_board.h"

#include "utils.h"

UNIT_TEST(test_board_shape);
UNIT_TEST(test_coord_on_board);
UNIT_TEST(test_get_expansions);

LIST_TESTS(
    test_board_shape,
    test_coord_on_board,
    test_get_expansions,
)

SUB_TEST(check_coord_on, board_t, board_coord_t, board_shape_t);
SUB_TEST(check_coord_off, board_t, board_coord_t, board_shape_t);
SUB_TEST(test_all_indices, board_t, expansion_index_t[MAX_EXPANSIONS], expansion_index_t, expansion_index_t);
SUB_TEST(verify_expansions, board_t, expansion_index_t[MAX_EXPANSIONS], size_t, expansion_index_t, size_t, expansion_index_t);

/* Unit tests */

UNIT_TEST(test_board_shape) {
    board_t board;
    board_load_info_t info;
    board_shape_t shape, res;

    shape.height = 56;
    shape.width = 34;

    info = new_info(shape);
    board = make_board(info);
    
    res = get_board_shape(board);

    ASSERT_TRUE(
        (shape.height == res.height) && (shape.width == res.width),
        "Expected shape (%lu, %lu), got (%lu, %lu)",
        shape.height, shape.width, res.height, res.width
    );

    TEST_END;
}

UNIT_TEST(test_coord_on_board) {
    size_t i, j;
    board_t board;
    board_load_info_t info;
    board_shape_t shape;
    bool exists;

    shape.height = 3;
    shape.width = 7;

    info = new_info(shape);
    board = make_board(info);

    /*
     * (i, j) \in (1..shape.height, 1..shape.width) is on the board.
     **/
    for (i = 0; i < shape.height; i++) {
        for (j = 0; j < shape.width; j++) {
            CALL_TEST(check_coord_on, board, make_coord(i + 1, j + 1), shape);
        }
    }

    /*
     * If (i, j) \nin (1..shape.height, 1..shape.width) is on the board.
     * Then it is not on the board.
     **/
    for (i = 0; i < shape.height + 2; i++) {
        CALL_TEST(check_coord_off, board, make_coord(i, 0), shape);
        CALL_TEST(check_coord_off, board, make_coord(i, shape.width + 1), shape);
    }

    for (j = 0; j < shape.width + 2; j++) {
        CALL_TEST(check_coord_off, board, make_coord(0, j), shape);
        CALL_TEST(check_coord_off, board, make_coord(shape.height + 1, j), shape);
    }

    TEST_END;
}


SUB_TEST(check_coord_on, board_t board, board_coord_t coords, board_shape_t shape) {
    bool exists = coord_on_board(board, coords);
    ASSERT_TRUE(
        exists, "Coordinate (%lu, %lu) should exist on board of size (%lu, %lu)",
        coords.col, coords.row, shape.height, shape.width
    );
    TEST_END;
}

SUB_TEST(check_coord_off, board_t board, board_coord_t coord, board_shape_t shape) {
    bool exists = coord_on_board(board, coord);
    ASSERT_FALSE(
        exists, "Coordinate (%lu, %lu) should not exist on board of size (%lu, %lu)",
        coord.col, coord.row, shape.height, shape.width
    );
    TEST_END;
}

UNIT_TEST(test_get_expansions) {
    board_t board;
    expansion_index_t exp1, exp2, expansions[MAX_EXPANSIONS];

    for (exp1 = e_STANDARD; exp1 < TOTAL_EXPANSIONS; exp1++) {
        for (exp2 = e_STANDARD; exp2 < TOTAL_EXPANSIONS; exp2++) {
            if (exp1 == exp2) continue;
            CALL_TEST(test_all_indices, board, expansions, exp1, exp2);
        }
    }

    TEST_END;
}


SUB_TEST(test_all_indices, board_t board, expansion_index_t expansions[MAX_EXPANSIONS], expansion_index_t exp1, expansion_index_t exp2) {
    size_t index1, index2;
    board_shape_t shape;
    board_load_info_t info;

    shape.height = 1;
    shape.width = 1;
    for (index1 = 0; index1 < MAX_EXPANSIONS; index1++) {
        for (index2 = 0; index2 < MAX_EXPANSIONS; index2++) {
            if (index1 == index2) continue;

            clear_expansion_list(expansions);
            expansions[index1] = exp1;
            expansions[index2] = exp2;

            info = new_info(shape);
            copy_expansions(info.expansions, expansions);
            board = make_board(info);

            CALL_TEST(verify_expansions, board, expansions, index1, exp1, index2, exp2);
        }
    }

    TEST_END;
}

SUB_TEST(verify_expansions, board_t board, expansion_index_t expansions[MAX_EXPANSIONS], size_t index1, expansion_index_t exp1, size_t index2, expansion_index_t exp2) {
    size_t i;
    bool in_info, in_board;

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

    TEST_END;
}

