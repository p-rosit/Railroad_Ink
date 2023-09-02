#include "../railroad_constants.h"
#include "railroad_data.h"

LOAD_EXPANSION_FUNCTION(standard, STANDARD_TILES,
    /*          Type        Connections                         Stations */
    /*  0 */    t_C,        c_C,    c_C,    c_E,    c_E,        s__,
    /*  1 */    t_C,        c_R,    c_R,    c_E,    c_E,        s__,
    /*  2 */    t_C,        c_C,    c_R,    c_E,    c_E,        sS_,
    /*  3 */    t_i,        c_C,    c_E,    c_E,    c_E,        sS_,
    /*  4 */    t_i,        c_R,    c_E,    c_E,    c_E,        sS_,
    /*  5 */    t_I,        c_C,    c_E,    c_C,    c_E,        s__,
    /*  6 */    t_I,        c_R,    c_E,    c_R,    c_E,        s__,
    /*  7 */    t_I,        c_C,    c_E,    c_R,    c_E,        sS_,
    /*  8 */    t_T,        c_C,    c_C,    c_C,    c_E,        s__,
    /*  9 */    t_T,        c_R,    c_R,    c_R,    c_E,        s__,
    /* 10 */    t_T,        c_C,    c_R,    c_C,    c_E,        sS_,
    /* 11 */    t_T,        c_R,    c_C,    c_R,    c_E,        sS_,
    /* 12 */    t_T,        c_C,    c_C,    c_R,    c_E,        sS_,
    /* 13 */    t_T,        c_C,    c_R,    c_R,    c_E,        sS_,
    /* 14 */    t_CC,       c_C,    c_C,    c_C,    c_C,        s__,
    /* 15 */    t_CC,       c_R,    c_R,    c_R,    c_R,        s__,
    /* 16 */    t_O,        c_C,    c_R,    c_C,    c_R,        s__,
)

LOAD_EXPANSION_FUNCTION(special, SPECIAL_TILES,
    /*          Type        Connections                         Stations */
    /* 0 */     t_E,        c_E,    c_E,    c_E,    c_E,        s__,
    /* 1 */     t_S,        c_C,    c_C,    c_C,    c_C,        s__,
    /* 2 */     t_S,        c_C,    c_C,    c_C,    c_R,        sS_,
    /* 3 */     t_S,        c_C,    c_C,    c_R,    c_R,        sS_,
    /* 4 */     t_S,        c_C,    c_R,    c_C,    c_R,        sS_,
    /* 5 */     t_S,        c_C,    c_R,    c_R,    c_R,        sS_,
    /* 6 */     t_S,        c_R,    c_R,    c_R,    c_R,        s__,
    /* 7 */     t_i,        c_C,    c_E,    c_E,    c_E,        s__,
    /* 8 */     t_i,        c_R,    c_E,    c_E,    c_E,        s__,
)

LOAD_EXPANSION_FUNCTION(mountain, MOUNTAIN_TILES,
    /*          Type        Connections                         Stations */
    /* 0 */     t_Ep,       c_M,    c_M,    c_M,    c_M,        s__,
    /* 1 */     t_C,        c_P,    c_P,    c_M,    c_M,        s__,
    /* 2 */     t_I,        c_P,    c_M,    c_P,    c_M,        s__,
)

LOAD_EXPANSION_FUNCTION(river, RIVER_TILES,
    /*          Type        Connections                         Stations */
    /* 0 */     t_C,        c_Ri,   c_Ri,   c_E,    c_E,        s__,
    /* 1 */     t_I,        c_Ri,   c_E,    c_Ri,   c_E,        s__,
    /* 2 */     t_O,        c_C,    c_Ri,   c_C,    c_Ri,       s__,
    /* 3 */     t_O,        c_R,    c_Ri,   c_R,    c_Ri,       s__,
)

LOAD_EXPANSION_FUNCTION(lake, LAKE_TILES,
    /*          Type        Connections                         Stations */
    /* 0 */     t_C,        c_La,   c_La,   c_E,    c_E,        s__,
    /* 1 */     t_i,        c_La,   c_E,    c_E,    c_E,        s__,
    /* 2 */     t_I,        c_C,    c_E,    c_La,   c_E,        sS_,
    /* 3 */     t_I,        c_R,    c_E,    c_La,   c_E,        sS_,
    /* 4 */     t_T,        c_La,   c_La,   c_La,   c_E,        s__,
    /* 5 */     t_S,        c_C,    c_R,    c_La,   c_La,       sS_,
)

LOAD_EXPANSION_FUNCTION(lava, LAVA_TILES,
    /*          Type        Connections                         Stations */
    /* 0 */     t_C,        c_Lv,   c_Lv,   c_Lv,   c_Lv,       s__,
    /* 1 */     t_I,        c_Lv,   c_E,    c_Lv,   c_E,        s__,
    /* 2 */     t_T,        c_Lv,   c_Lv,   c_Lv,   c_E,        s__,
    /* 3 */     t_dT,       c_C,    c_Lv,   c_C,    c_E,        s__,
    /* 4 */     t_dT,       c_R,    c_Lv,   c_R,    c_E,        s__,
)

LOAD_EXPANSION_FUNCTION(meteor, METEOR_TILES,
    /*          Type        Connections                         Stations */
    /* 0 */     t_E,        c_E,    c_E,    c_E,    c_E,        s__,
    /* 1 */     t_Ep,       c_P,    c_E,    c_E,    c_E,        s__,
    /* 2 */     t_Ep,       c_E,    c_P,    c_E,    c_E,        s__,
    /* 3 */     t_Ep,       c_E,    c_E,    c_P,    c_E,        s__,
    /* 4 */     t_Ep,       c_E,    c_E,    c_E,    c_P,        s__,
    /* 5 */     t_Ep,       c_P,    c_P,    c_P,    c_P,        s__,
    /* 6 */     t_Ep,       c_P,    c_E,    c_E,    c_E,        s__,
    /* 7 */     t_Ep,       c_P,    c_P,    c_E,    c_E,        s__,
    /* 8 */     t_Ep,       c_P,    c_P,    c_P,    c_E,        s__,
)

