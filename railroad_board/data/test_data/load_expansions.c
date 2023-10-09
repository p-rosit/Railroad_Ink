#include "../../../cut/cut.h"

#include "../railroad_data.h"

#define LOAD_TEST(test_name, EXPANSION_INDEX, TILE_AMOUNT)                      \
    UNIT_TEST(test_name) {                                                      \
        tile_t tile;                                                            \
        expansion_index_t expansions[MAX_EXPANSIONS];                           \
        clear_expansion_list(expansions);                                       \
                                                                                \
        expansions[0] = EXPANSION_INDEX;                                        \
        const game_data_t game_data = load_data(expansions);                    \
                                                                                \
        ASSERT_NOT_NULL(                                                        \
            game_data.tile_data,                                                \
            "No tile data pointer was allocted."                                \
        );                                                                      \
                                                                                \
        for (size_t i = 0; i < TILE_AMOUNT; i++) {                              \
            tile = load_tile(game_data, make_tile_data(EXPANSION_INDEX, 0, i)); \
            ASSERT_EQUAL(                                                       \
                tile.expansion_index,                                           \
                EXPANSION_INDEX,                                                \
                "Tile does not come from correct "                              \
                "expansion. Expected %lu, got %lu.",                            \
                EXPANSION_INDEX, tile.expansion_index                           \
            );                                                                  \
            ASSERT_EQUAL(                                                       \
                tile.local_index,                                               \
                i,                                                              \
                "Tile does not have correct local "                             \
                "index. Expected %lu, got %lu.",                                \
                i, tile.local_index                                             \
            );                                                                  \
            VERIFY_TILE(i, expansion, tile);                                    \
        }                                                                       \
                                                                                \
        TEST_END;                                                               \
    }

#define VERIFY_TILE(index, expansion, tile) \
    do {                                                                        \
        size_t j, sum;                                                          \
                                                                                \
        ASSERT_EQUAL(                                                           \
            tile.orientation, NORTH,                                            \
            "Tile %lu should point north when loaded. Pointing %lu.",           \
            index, tile.orientation                                             \
        );                                                                      \
                                                                                \
        ASSERT_EQUAL(                                                           \
            tile.center, false,                                                 \
            "Tile %lu should not be a center tile when loaded, got %lu.",       \
            index, tile.center                                                  \
        );                                                                      \
                                                                                \
        for (j = 0; j < DIRECTIONS; j++) {                                      \
            ASSERT_TRUE(                                                        \
                tile.connections[j] < TOTAL_CONNECTIONS,                        \
                "Tile %lu, Invalid connection saved, got %lu.",                 \
                index, tile.connections[j]                                      \
            );                                                                  \
        }                                                                       \
                                                                                \
        for (j = 0, sum = 0; j < MAX_TILE_NETWORKS * DIRECTIONS; j++) {         \
            sum += tile.networks[j];                                            \
        }                                                                       \
                                                                                \
        ASSERT_TRUE(                                                            \
            sum <= DIRECTIONS,                                                  \
            "Tile %lu: Some directions belong to more than "                    \
            "one inner tile network.",                                          \
            index                                                               \
        );                                                                      \
                                                                                \
        for (j = 0; j < MAX_TYPE_EXPANSIONS; j++) {                             \
            ASSERT_EQUAL(                                                       \
                tile.data[j], NO_TYPE,                                          \
                "Tile %lu: type information has already been "                  \
                "filled in. Got %lu.",                                          \
                index, tile.data[j]                                             \
            );                                                                  \
        }                                                                       \
                                                                                \
    } while (0) 

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
    load_meteor,
    // load_forest,
    // load_trail,
    // load_desert,
    // load_canyon
)

