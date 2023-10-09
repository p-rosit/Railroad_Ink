#include "../../../cut/cut.h"

#include "../railroad_data.h"

void clear_expansion_list(expansion_index_t* expansions) {
    for (size_t i = 0; i < MAX_EXPANSIONS; i++) {
        expansions[i] = NO_EXPANSION;
    }
}


tile_load_data_t make_tile_data(expansion_index_t exp, expansion_index_t local_exp, size_t local) {
    return (tile_load_data_t) {
        .global_expansion_index = exp,
        .local_expansion_index = local_exp,
        .local_index = local
    };
}


UNIT_TEST(load_no_expansion) {
    expansion_index_t expansions[MAX_EXPANSIONS];
    clear_expansion_list(expansions);

    const game_data_t game_data = load_data(expansions);

    free_data(game_data);
    TEST_END;
}

UNIT_TEST(load_max_expansions) {
    size_t index;
    expansion_index_t expansion_index;
    expansion_index_t expansions[MAX_EXPANSIONS];

    clear_expansion_list(expansions);

    index = 0;
    expansion_index = 0;
    while (index < MAX_EXPANSIONS && expansion_index < TOTAL_EXPANSIONS) {
        if (true) {
            expansions[index++] = expansion_index;
        }
        expansion_index += 1;
    }

    const game_data_t game_data = load_data(expansions);

    free_data(game_data);
    TEST_END;
}

LIST_TESTS(
    load_no_expansion,
    load_max_expansions
)

