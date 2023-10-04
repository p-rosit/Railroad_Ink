#include "../../../cut/cut.h"

#include "../railroad_data.h"

#define LOAD_TEST(name, expansion_index, tile_amount)                           \
    UNIT_TEST(name) {                                                           \
        tile_t tile;                                                            \
        expansion_index_t expansions[MAX_EXPANSIONS];                           \
        clear_expansion_list(expansions);                                       \
                                                                                \
        expansions[0] = expansion_index;                                        \
        const game_data_t game_data = load_data(expansions);                    \
                                                                                \
        ASSERT_NOT_NULL(                                                        \
            game_data.tile_data,                                                \
            "No tile data pointer was allocted."                                \
        );                                                                      \
                                                                                \
        for (size_t i = 0; i < tile_amount; i++) {                              \
            tile = load_tile(game_data, make_tile_data(e_STANDARD, i));         \
        }                                                                       \
                                                                                \
        TEST_END;                                                               \
    }

void clear_expansion_list(expansion_index_t* expansions) {
    for (size_t i = 0; i < MAX_EXPANSIONS; i++) {
        expansions[i] = NO_EXPANSION;
    }
}


tile_data_t make_tile_data(expansion_index_t exp, size_t local) {
    return (tile_data_t) {
        .expansion_index = exp,
        .local_index = local
    };
}

LOAD_TEST(load_standard,    e_STANDARD,     STANDARD_TILES)
LOAD_TEST(load_special,     e_SPECIAL,      SPECIAL_TILES)
LOAD_TEST(load_mountain,    e_MOUNTAIN,     MOUNTAIN_TILES)
LOAD_TEST(load_river,       e_RIVER,        RIVER_TILES)
LOAD_TEST(load_lake,        e_LAKE,         LAKE_TILES)
LOAD_TEST(load_meteor,      e_METEOR,       METEOR_TILES)
// LOAD_TEST(load_forest,      e_FOREST,       FOREST_TILES)
// LOAD_TEST(load_trail,       e_TRAIL,        TRAIL_TILES)
// LOAD_TEST(load_desert,      e_DESERT,       DESERT_TILES)
// LOAD_TEST(load_canyon,      e_CANYON,       CANYON_TILES)

LIST_TESTS(
    load_standard,
    load_special,
    load_mountain,
    load_river,
    load_lake,
    load_meteor
)
