#include "railroad_types.h"

/* Version */
#define DATA_VERSION            (1)

/* Settings */
#define MAX_COMBINATIONS        (4)

/* Tile information */
#define TOTAL_TILES             (100)
#define TOTAL_CONNECTIONS       (8)
#define TOTAL_DICE              (10)

/************************************************************
*                                                           *
*   Data structures keeping track of all the information    *
*   needed to play the game.                                *
*                                                           *
************************************************************/

enum expansion {
    expansion_STANDARD,
    expansion_SPECIAL,
    expansion_NETWORK,
    expansion_LONGEST,
    expansion_CENTER,
    expansion_ERROR,
    expansion_BUILDING,
    expansion_MOUNTAIN,
    expansion_RIVER,
    TOTAL_EXPANSIONS,
};

enum tile_types {
    tile_E,
    tile_Ep,
    tile_C,
    tile_i,
    tile_I,
    tile_T,
    tile_dT,
    tile_CC,
    tile_O,
    tile_S,
    TOTAL_TYPES,
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
    const board_data_t tiles[TOTAL_TILES];
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
    const size_t dice[TOTAL_DICE];
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
        0,  /* Standard */
        4,  /*  Special */
    },
    .tiles = {
        /*                  Type     Connections    Stations */
        /* 0 Standard */    2,       1, 1, 0, 0,    0,
        /* 1          */    2,       2, 2, 0, 0,    0,
        /* 2          */    2,       1, 2, 0, 0,    1,
        /* 3          */    3,       1, 0, 0, 0,    1,
        /* 4          */    3,       2, 0, 0, 0,    1,
        /* 5          */    4,       1, 0, 1, 0,    0,
    },
};

const DiceData_t dice_data = {
    .dice = {
        /* Faces                1       2       3       5       6 */
        /* classic_simple */    0, 0,   0, 0,   0, 0,   0, 0,   0, 0,
    },
};

const GameMapping_t game_mapping = {

};
