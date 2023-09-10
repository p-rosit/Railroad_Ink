#ifndef RAILROAD_EXPANSIONS_H
#define RAILROAD_EXPANSIONS_H

#include "railroad_types.h"

#define MAX_EXPANSIONS      (9)
#define MAX_TYPE_EXPANSIONS (3)

#define PROTOTYPE_EXPANSION_HAS(name) \
bool expansion_has_##name(expansion_index_t expansion)

#define EXPANSION_HAS(name, ...) \
bool expansion_has_##name(expansion_index_t expansion) { \
    const bool data[TOTAL_EXPANSIONS] = {__VA_ARGS__}; \
    return data[expansion]; \
}

enum expansion {
    e_STANDARD,
    e_SPECIAL,
    e_NETWORK,
    e_LONGEST,
    e_CENTER,
    e_ERROR,
    e_BUILDING,

    e_MOUNTAIN,
    e_RIVER,
    e_LAKE,
    e_LAVA,
    e_METEOR,
    e_FOREST,
    e_TRAIL,
    e_DESERT,
    e_CANYON,
    TOTAL_EXPANSIONS,
    NO_EXPANSION,
};

PROTOTYPE_EXPANSION_HAS(tiles);

PROTOTYPE_EXPANSION_HAS(types);

#endif

