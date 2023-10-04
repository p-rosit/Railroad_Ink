#include "../../../cut/cut.h"

#include "../railroad_data.h"

void clear_expansion_list(expansion_index_t* expansions) {
    for (size_t i = 0; i < MAX_EXPANSIONS; i++) {
        expansions[i] = NO_EXPANSION;
    }
}


UNIT_TEST(load_no_expansion) {
    expansion_index_t expansions[MAX_EXPANSIONS];
    clear_expansion_list(expansions);

    const game_data_t game_data = load_data(expansions);

    TEST_END;
}

UNIT_TEST(load_standard) {
    expansion_index_t expansions[MAX_EXPANSIONS];
    clear_expansion_list(expansions);

    expansions[0] = e_STANDARD;
    const game_data_t game_data = load_data(expansions);

    ASSERT_NOT_NULL(game_data.tile_data, "No tile data pointer was allocted.");

    TEST_END;
}

LIST_TESTS(
    load_no_expansion,
    load_standard
)

