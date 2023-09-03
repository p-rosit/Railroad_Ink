#include <stdio.h>
#include <stdarg.h>
#include "../railroad_types.h"
#include "../railroad_constants.h"
#include "expansion_data.c"
#include "railroad_data.h"

size_t count_expansion_tiles(expansion_index_t);
size_t load_expansion(board_data_t*, expansion_index_t, size_t);


size_t get_expansions(va_list ap, size_t expansion_amount, expansion_index_t expansions[DEFAULT_EXPANSIONS + MAX_EXPANSIONS]) {
    size_t i, extracted_values;
    expansion_index_t temp;

    i = 0;
    expansions[i++] = e_STANDARD;
    expansions[i++] = e_SPECIAL;

    for (extracted_values = 0; i < DEFAULT_EXPANSIONS + MAX_EXPANSIONS && extracted_values < expansion_amount; i++, extracted_values++) {
        temp = va_arg(ap, expansion_index_t);
        if (temp == e_STANDARD || temp == e_SPECIAL) {
            printf("WARNING: Loading default expansion with index %lu explicitly.\n", temp);
            i -= 1;
        } else {
            expansions[i] = temp;
        }
    }

    if (extracted_values != expansion_amount) {
        printf("WARNING: More than %d expansions have been specified, ignoring excess.\n", MAX_EXPANSIONS);
    }

    for (; i < DEFAULT_EXPANSIONS + MAX_EXPANSIONS; i++) {
        expansions[i] = NO_EXPANSION;
    }

    return expansion_amount;
}


const GameData_t load_data(size_t expansion_amount, ...) {
    va_list ap;
    size_t i, tile_amount, index, expansion_index[DEFAULT_EXPANSIONS + MAX_EXPANSIONS];
    expansion_index_t temp, expansions[DEFAULT_EXPANSIONS + MAX_EXPANSIONS];
    board_data_t* tile_data;

    va_start(ap, expansion_amount);
    get_expansions(ap, expansion_amount, expansions);
    va_end(ap);

    tile_amount = 0;
    for (i = 0; i < DEFAULT_EXPANSIONS + MAX_EXPANSIONS; i++) {
        tile_amount += count_expansion_tiles(expansions[i]);
    }

    index = 0;
    tile_data = malloc(TILE_DATA_WIDTH * tile_amount * sizeof(board_data_t));
    for (i = 0; i < DEFAULT_EXPANSIONS + MAX_EXPANSIONS; i++) {
        expansion_index[i] = index;
        index = load_expansion(tile_data, expansions[i], index);
    }

    GameData_t game_data = (GameData_t) {
        .connection_data = (ConnectionData_t) {
            .traversable = {
                0,  /*  E */
                1,  /*  C */
                1,  /*  R */
                0,  /*  M */
                0,  /*  P */
                0,  /* Ri */
                0,  /* La */
                0,  /* Lv */
            },
            .non_connections = {
                1,  /*  E */
                0,  /*  C */
                0,  /*  R */
                1,  /*  M */
                1,  /*  P */
                0,  /* Ri */
                0,  /* La */
                0,  /* Lv */
            },
            .valid_connections = {
                /*          E   C   R   M   P   Ri  La  Lv  */
                /*  E */    1,  1,  1,  1,  1,  1,  1,  1,
                /*  C */    1,  1,  0,  1,  1,  0,  0,  0,
                /*  R */    1,  0,  1,  1,  1,  0,  0,  0,
                /*  M */    1,  0,  0,  0,  0,  0,  0,  0,
                /*  P */    0,  1,  1,  0,  0,  1,  0,  0,
                /* Ri */    1,  0,  0,  1,  1,  1,  0,  0,
                /* La */    1,  0,  0,  1,  1,  0,  1,  0,
                /* Lv */    1,  0,  0,  1,  1,  0,  0,  1,
            },
            .networks = {
                /*          First network       Second network */
                /*  E */    0,  0,  0,  0,      0,  0,  0,  0,
                /* Ep */    0,  0,  0,  0,      0,  0,  0,  0,
                /*  C */    1,  1,  0,  0,      0,  0,  0,  0,
                /*  i */    1,  0,  0,  0,      0,  0,  0,  0, 
                /*  I */    1,  0,  1,  0,      0,  0,  0,  0,
                /*  T */    1,  1,  1,  0,      0,  0,  0,  0,
                /* dT */    1,  0,  1,  0,      0,  1,  0,  0,
                /* CC */    1,  1,  0,  0,      0,  0,  1,  1,
                /*  O */    1,  0,  1,  0,      0,  1,  0,  1,
                /*  S */    1,  1,  1,  1,      0,  0,  0,  0,
            },
        },
        .tile_data = tile_data,
    };

    for (i = 0; i < DEFAULT_EXPANSIONS + MAX_EXPANSIONS; i++) {
        ((expansion_index_t*) game_data.expansion_index)[i] = expansion_index[i];
    }

    return game_data;
}

void free_data(const GameData_t game_data) {
    free((board_data_t*) game_data.tile_data);
}

Tile_t load_tile(const GameData_t game_data, size_t expansion_index, size_t local_index) {
    return (Tile_t) {};
}

size_t load_expansion(board_data_t* tile_data, expansion_index_t expansion, size_t index) {
    switch (expansion) {
        case e_STANDARD:
            return load_standard_tiles(tile_data, index);
        case e_SPECIAL:
            return load_special_tiles(tile_data, index);
        case e_MOUNTAIN:
            return load_mountain_tiles(tile_data, index);

        case e_RIVER:
            return load_river_tiles(tile_data, index);
        case e_LAKE:
            return load_lake_tiles(tile_data, index);

        case e_LAVA:
            return load_lava_tiles(tile_data, index);
        case e_METEOR:
            return load_meteor_tiles(tile_data, index);

        case NO_EXPANSION:
            return index;
        default:
            printf("WARNING: Trying to load unknown expansion with index %lu.\n", expansion);
            return index;
    }
}

size_t count_expansion_tiles(expansion_index_t expansion) {
    switch (expansion) {
        case e_STANDARD:
            return STANDARD_TILES;
        case e_SPECIAL:
            return SPECIAL_TILES;
        case e_MOUNTAIN:
            return MOUNTAIN_TILES;

        case e_RIVER:
            return RIVER_TILES;
        case e_LAKE:
            return LAKE_TILES;

        case e_LAVA:
            return LAVA_TILES; 
        case e_METEOR:
            return METEOR_TILES;

        case NO_EXPANSION:
            return 0;
        default:
            printf("WARNING: Trying to count tiles of unknown expansion with index %lu.\n", expansion);
            return 0;
    }
}

