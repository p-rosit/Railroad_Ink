#ifndef RAILROAD_CONSTANTS_H
#define RAILROAD_CONSTANTS_H

#define DIRECTIONS          (4)
#define MAX_TILE_NETWORKS   (2)


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

