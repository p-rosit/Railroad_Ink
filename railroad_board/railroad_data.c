#include "railroad_types.h"

/* Version */
#define DATA_VERSION            (1)

/* Settings */
#define MAX_COMBINATIONS        (4)

/* Tile information */
#define TOTAL_TILES             (37)
#define TOTAL_DICE              (5)

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
    s___,
    s_S_,
    s__S,
    s_SS,
};

/**
 * Data for all the tiles.
 *
 *      expansion_index:    The index of the first tile for each expansion.
 *      tiles:              The tiles for each expansions, tile i has the
 *                          following information:
 *
 *                              tiles[i, 0] = tile_type
 *                              tiles[i, 1:5] = connections
 *                              tiles[i, 6] = stations
 */
typedef struct TileData {
    /* Tile data */
    const bool traversable[TOTAL_CONNECTIONS];
    const bool non_connections[TOTAL_CONNECTIONS];
    const bool valid_connections[TOTAL_CONNECTIONS * TOTAL_CONNECTIONS];
    const bool networks[2 * 4 * TOTAL_TYPES];

    /* Tiles */
    const size_t expansion_index[TOTAL_EXPANSIONS];
    const board_data_t tiles[6 * TOTAL_TILES];
} TileData_t;

/**
 *  Data for all the dice.
 *
 *      dice:   Dice data, dice i has the following information
 *
 *                  dice[i, 0] = expansion
 *                  dice[i, 1] = index in expansion
 *
 */
typedef struct DiceData {
    const size_t dice[2 * 6 * TOTAL_DICE];
} DiceData_t;

/**
 * Contains all the mappings between names and integers
 */
typedef struct GameMapping {
    void* expansions;
} GameMapping_t;

/**
 *
 */
const TileData_t tile_data = {
    .traversable = {
        /* E  */ 0, 
        /* C  */ 1, 
        /* R  */ 1, 
        /* M  */ 0, 
        /* P  */ 0, 
        /* Ri */ 0, 
        /* La */ 0, 
        /* Lv */ 0, 
    },
    .non_connections = {
        /* E  */ 1, 
        /* C  */ 0, 
        /* R  */ 0, 
        /* M  */ 1, 
        /* P  */ 1, 
        /* Ri */ 0, 
        /* La */ 0, 
        /* Lv */ 0,
    },
    .valid_connections = {
        /*        E,  C,  R,  M,  P, Ri, La, Lv */
        /* E  */  1,  1,  1,  1,  1,  1,  1,  1,
        /* C  */  1,  1,  0,  1,  1,  0,  0,  0,
        /* R  */  1,  0,  1,  1,  1,  0,  0,  0,
        /* M  */  1,  0,  0,  0,  0,  0,  0,  0,
        /* P  */  0,  1,  1,  0,  0,  0,  0,  0,
        /* Ri */  1,  0,  0,  1,  1,  1,  0,  0,
        /* La */  1,  0,  0,  1,  1,  0,  1,  0,
        /* Lv */  1,  0,  0,  1,  1,  0,  0,  1,
    },
    .networks = {
        /*          First           Second */
        /* E  */    0, 0, 0, 0,     0, 0, 0, 0,    
        /* Ep */    0, 0, 0, 0,     0, 0, 0, 0,    
        /* C  */    1, 1, 0, 0,     0, 0, 0, 0,    
        /* i  */    1, 0, 0, 0,     0, 0, 0, 0,    
        /* I  */    1, 0, 1, 0,     0, 0, 0, 0,    
        /* T  */    1, 1, 1, 0,     0, 0, 0, 0,    
        /* dT */    1, 0, 1, 0,     0, 1, 0, 0,    
        /* CC */    1, 1, 0, 0,     0, 0, 1, 1,    
        /* O  */    1, 0, 1, 0,     0, 1, 0, 1,    
        /* S  */    1, 1, 1, 1,     0, 0, 0, 0,    
    },
    .expansion_index = {
        0,      /* Standard */
        17,     /* Special  */
        24,     /* Network  */
        24,     /* Longest  */
        24,     /* Center   */
        24,     /* Error    */
        24,     /* Building */
        24,     /* Mountain */
        27,     /* River */
        31,     /* Lake */
        37,     /* Lava */
        // 27,     /* Meteor */
        // 27,     /* Forest */
        // 27,     /* Trail */
        // 27,     /* Desert */
        // 27,     /* Canyon */
    },
    .tiles = {
        /*                  Type    Connections                 Stations */
        /* Standard */
        /*   0  0   */       t_C,    c_C,  c_C,  c_E,  c_E,     s___,
        /*   1  1   */       t_C,    c_R,  c_R,  c_E,  c_E,     s___,
        /*   2  2   */       t_C,    c_C,  c_R,  c_E,  c_E,     s_S_,
        /*   3  3   */       t_i,    c_C,  c_E,  c_E,  c_E,     s_S_,
        /*   4  4   */       t_i,    c_R,  c_E,  c_E,  c_E,     s_S_,
        /*   5  5   */       t_I,    c_C,  c_E,  c_C,  c_E,     s___,
        /*   6  6   */       t_I,    c_R,  c_E,  c_R,  c_E,     s___,
        /*   7  7   */       t_I,    c_C,  c_E,  c_R,  c_E,     s_S_,
        /*   8  8   */       t_T,    c_C,  c_C,  c_C,  c_E,     s___,
        /*   9  9   */       t_T,    c_R,  c_R,  c_R,  c_E,     s___,
        /*  10 10   */       t_T,    c_C,  c_R,  c_C,  c_E,     s_S_,
        /*  11 11   */       t_T,    c_R,  c_C,  c_R,  c_E,     s_S_,
        /*  12 12   */       t_T,    c_C,  c_C,  c_R,  c_E,     s_S_,
        /*  13 13   */       t_T,    c_C,  c_R,  c_R,  c_E,     s_S_,
        /*  14 14   */      t_CC,    c_C,  c_C,  c_C,  c_C,     s___,
        /*  15 15   */      t_CC,    c_R,  c_R,  c_R,  c_R,     s___,
        /*  16 16   */       t_O,    c_C,  c_R,  c_C,  c_R,     s___,

        /* Special */
        /*  17  0   */       t_E,    c_E,  c_E,  c_E,  c_E,     s___,
        /*  18  1   */       t_S,    c_C,  c_C,  c_C,  c_C,     s___,
        /*  19  2   */       t_S,    c_C,  c_C,  c_C,  c_R,     s_S_,
        /*  20  3   */       t_S,    c_C,  c_C,  c_R,  c_R,     s_S_,
        /*  21  4   */       t_S,    c_C,  c_R,  c_C,  c_R,     s_S_,
        /*  22  5   */       t_S,    c_C,  c_R,  c_R,  c_R,     s_S_,
        /*  23  6   */       t_S,    c_R,  c_R,  c_R,  c_R,     s___,

        /* Mountain */
        /*  24 0    */      t_Ep,    c_M,  c_M,  c_M,  c_M,     s___,
        /*  25 1    */       t_C,    c_P,  c_P,  c_M,  c_M,     s___,
        /*  26 2    */       t_I,    c_P,  c_M,  c_P,  c_M,     s___,

        /* River */
        /*  27 0    */       t_C,   c_Ri, c_Ri,  c_E,  c_E,     s___,
        /*  28 1    */       t_I,   c_Ri,  c_E, c_Ri,  c_E,     s___,
        /*  29 2    */       t_O,    c_C, c_Ri,  c_C, c_Ri,     s___,
        /*  30 3    */       t_O,    c_R, c_Ri,  c_R, c_Ri,     s___,

        /* Lake */
        /*  31 0    */       t_C,   c_La, c_La,  c_E,  c_E,     s___,
        /*  32 1    */       t_i,   c_La,  c_E,  c_E,  c_E,     s___,
        /*  33 2    */       t_I,    c_C,  c_E, c_La,  c_E,     s_S_,
        /*  34 3    */       t_I,    c_R,  c_E, c_La,  c_E,     s_S_,
        /*  35 4    */       t_T,   c_La, c_La, c_La,  c_E,     s___,
        /*  36 5    */       t_S,    c_C,  c_R, c_La, c_La,     s_S_,
    },
};

const DiceData_t dice_data = {
    .dice = {
        /* Faces                    1       2       3       4       5       6 */
        /* classic_simple       */   0,  0,  0,  1,  0,  5,  0,  6,  0,  8,  0,  9,
        /* classic_special      */   0,  2,  0,  2,  0,  7,  0,  7,  0, 16,  0, 16,
        /* challenge_simple     */   0,  8,  0,  9,  0, 14,  0, 15,  0, 16,  0, 16,
        /* challenge_special    */   0,  3,  0,  4,  0,  2,  0,  7,  0, 10,  0, 11,

        /* special_dice         */   1,  1,  1,  2,  1,  3,  1,  4,  1,  5,  1,  6,
    },
};

const GameMapping_t game_mapping = {

};

const TileData_t* get_tile_data() {
    return &tile_data;
}

const DiceData_t* get_dice_data() {
    return &dice_data;
}

const GameMapping_t* get_game_mapping() {
    return &game_mapping;
}

