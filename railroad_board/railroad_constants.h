#ifndef RAILROAD_ENUMS_H
#define RAILROAD_ENUMS_H

/* Game constants */
#define DEFAULT_EXPANSIONS  (2)
#define MAX_EXPANSIONS      (3)
#define DIRECTIONS          (4)
#define MAX_TILE_NETWORKS   (2)

/* Tile information */
// #define TOTAL_TILES         (53)
// #define TOTAL_DICE          (5)

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

enum tile_types {
    t_E,
    t_Ep,
    t_C,
    t_i,
    t_I,
    t_T,
    t_dT,
    t_CC,
    t_O,
    t_S,
    TOTAL_TYPES,
};

enum connections {
    c_E,
    c_C,
    c_R,
    c_M,
    c_P,
    c_Ri,
    c_La,
    c_Lv,
    TOTAL_CONNECTIONS,
};

enum stations {
    s__,
    sS_,
    s_S,
    sSS,
};

#endif

