#ifndef TEST_BOARD_UTILS_H
#include <string.h>

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

board_load_info_t new_info(board_shape_t shape) {
    board_load_info_t info;

    info.shape = shape;
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
#endif

