#include "railroad_enums.h"
#include "railroad_types.h"
#include "railroad_structs.h"

ConnectionData_t connection_data = {
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
};

/**
 *
 */
TileData_t tile_data = {
    .expansion_index = {
        0,      /* Standard */
        17,     /* Special  */
        26,     /* Network  */
        26,     /* Longest  */
        26,     /* Center   */
        26,     /* Error    */
        26,     /* Building */
        26,     /* Mountain */
        29,     /* River    */
        33,     /* Lake     */
        39,     /* Lava     */
        44,     /* Meteor   */
        // 27,     /* Forest */
        // 27,     /* Trail */
        // 27,     /* Desert */
        // 27,     /* Canyon */
    },
    .tiles = {
        /*                  Type    Connections                 Stations */
        /* Standard */
        /*   0  0   */       t_C,    c_C,  c_C,  c_E,  c_E,     s__,
        /*   1  1   */       t_C,    c_R,  c_R,  c_E,  c_E,     s__,
        /*   2  2   */       t_C,    c_C,  c_R,  c_E,  c_E,     sS_,
        /*   3  3   */       t_i,    c_C,  c_E,  c_E,  c_E,     sS_,
        /*   4  4   */       t_i,    c_R,  c_E,  c_E,  c_E,     sS_,
        /*   5  5   */       t_I,    c_C,  c_E,  c_C,  c_E,     s__,
        /*   6  6   */       t_I,    c_R,  c_E,  c_R,  c_E,     s__,
        /*   7  7   */       t_I,    c_C,  c_E,  c_R,  c_E,     sS_,
        /*   8  8   */       t_T,    c_C,  c_C,  c_C,  c_E,     s__,
        /*   9  9   */       t_T,    c_R,  c_R,  c_R,  c_E,     s__,
        /*  10 10   */       t_T,    c_C,  c_R,  c_C,  c_E,     sS_,
        /*  11 11   */       t_T,    c_R,  c_C,  c_R,  c_E,     sS_,
        /*  12 12   */       t_T,    c_C,  c_C,  c_R,  c_E,     sS_,
        /*  13 13   */       t_T,    c_C,  c_R,  c_R,  c_E,     sS_,
        /*  14 14   */      t_CC,    c_C,  c_C,  c_C,  c_C,     s__,
        /*  15 15   */      t_CC,    c_R,  c_R,  c_R,  c_R,     s__,
        /*  16 16   */       t_O,    c_C,  c_R,  c_C,  c_R,     s__,

        /* Special */
        /*  17  0   */       t_E,    c_E,  c_E,  c_E,  c_E,     s__,
        /*  18  1   */       t_S,    c_C,  c_C,  c_C,  c_C,     s__,
        /*  19  2   */       t_S,    c_C,  c_C,  c_C,  c_R,     sS_,
        /*  20  3   */       t_S,    c_C,  c_C,  c_R,  c_R,     sS_,
        /*  21  4   */       t_S,    c_C,  c_R,  c_C,  c_R,     sS_,
        /*  22  5   */       t_S,    c_C,  c_R,  c_R,  c_R,     sS_,
        /*  23  6   */       t_S,    c_R,  c_R,  c_R,  c_R,     s__,
        /*  24  7   */       t_i,    c_C,  c_E,  c_E,  c_E,     s__,
        /*  25  8   */       t_i,    c_R,  c_E,  c_E,  c_E,     s__,

        /* Mountain */
        /*  26 0    */      t_Ep,    c_M,  c_M,  c_M,  c_M,     s__,
        /*  27 1    */       t_C,    c_P,  c_P,  c_M,  c_M,     s__,
        /*  28 2    */       t_I,    c_P,  c_M,  c_P,  c_M,     s__,

        /* River */
        /*  29 0    */       t_C,   c_Ri, c_Ri,  c_E,  c_E,     s__,
        /*  30 1    */       t_I,   c_Ri,  c_E, c_Ri,  c_E,     s__,
        /*  31 2    */       t_O,    c_C, c_Ri,  c_C, c_Ri,     s__,
        /*  32 3    */       t_O,    c_R, c_Ri,  c_R, c_Ri,     s__,

        /* Lake */
        /*  33 0    */       t_C,   c_La, c_La,  c_E,  c_E,     s__,
        /*  34 1    */       t_i,   c_La,  c_E,  c_E,  c_E,     s__,
        /*  35 2    */       t_I,    c_C,  c_E, c_La,  c_E,     sS_,
        /*  36 3    */       t_I,    c_R,  c_E, c_La,  c_E,     sS_,
        /*  37 4    */       t_T,   c_La, c_La, c_La,  c_E,     s__,
        /*  38 5    */       t_S,    c_C,  c_R, c_La, c_La,     sS_,

        /* Lava */
        /*  39 0    */       t_C,   c_Lv, c_Lv, c_Lv, c_Lv,     s__,
        /*  40 1    */       t_I,   c_Lv,  c_E, c_Lv,  c_E,     s__,
        /*  41 2    */       t_T,   c_Lv, c_Lv, c_Lv,  c_E,     s__,
        /*  42 3    */      t_dT,    c_C, c_Lv,  c_C,  c_E,     s__,
        /*  43 4    */      t_dT,    c_R, c_Lv,  c_R,  c_E,     s__,

        /* Meteor */
        /*  44 0    */       t_E,    c_E,  c_E,  c_E,  c_E,     s__,
        /*  45 1    */      t_Ep,    c_P,  c_E,  c_E,  c_E,     s__,
        /*  46 2    */      t_Ep,    c_E,  c_P,  c_E,  c_E,     s__,
        /*  47 3    */      t_Ep,    c_E,  c_E,  c_P,  c_E,     s__,
        /*  48 4    */      t_Ep,    c_E,  c_E,  c_E,  c_P,     s__,
        /*  49 5    */      t_Ep,    c_P,  c_P,  c_P,  c_P,     s__,
        /*  50 6    */      t_Ep,    c_P,  c_E,  c_E,  c_E,     s__,
        /*  51 7    */      t_Ep,    c_P,  c_P,  c_E,  c_E,     s__,
        /*  52 8    */      t_Ep,    c_P,  c_P,  c_P,  c_E,     s__,
    },
};

DiceData_t dice_data = {
    .dice = {
        /* Faces                    1       2       3       4       5       6 */
        /* classic_simple       */   0,  0,  0,  1,  0,  5,  0,  6,  0,  8,  0,  9,
        /* classic_special      */   0,  2,  0,  2,  0,  7,  0,  7,  0, 16,  0, 16,
        /* challenge_simple     */   0,  8,  0,  9,  0, 14,  0, 15,  0, 16,  0, 16,
        /* challenge_special    */   0,  3,  0,  4,  0,  2,  0,  7,  0, 10,  0, 11,

        /* special_dice         */   1,  1,  1,  2,  1,  3,  1,  4,  1,  5,  1,  6,
    },
};

GameMapping_t game_mapping = {

};

TileData_t* load_tile_data() {
    return &tile_data;
}

DiceData_t* load_dice_data() {
    return &dice_data;
}

GameMapping_t* load_game_mapping() {
    return &game_mapping;
}

const board_data_t* load_tile_connections(TileData_t* tile_data, size_t index) {
    return tile_data->tiles + 6 * index;
}

const bool* load_tile_networks(ConnectionData_t* connection_data, size_t index) {
    return connection_data->networks + 8 * index;
}
