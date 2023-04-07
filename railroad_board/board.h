#ifndef BOARD_H
#define BOARD_H

#include <stdint.h>
#include "mechanics/expansion.h"

/**
 * \brief       Represents the game board.
 *
 *              The `width` and `height` fields are the width and height of the board,
 *              respectively. The actual state of the board is stored in the `board`
 *              field. The board is a flattened three-dimensional array of size (height +
 *              2) x (width + 2) x 3, where each tile has three values that determine its
 *              state: id, type, and orientation.
 *
 *              - id: An integer that denotes the type of tile occupying the space. The
 *                    type can be empty, a straight road, a curved railroad, and so on.
 *              - type: An integer that encodes any extra information about the tile,
 *                      such as whether it contains a mountain or a cactus.
 *              - orientation: An integer that encodes the orientation of the tile; there
 *                             are eight possible orientations (including mirror images).
 */
typedef struct Board {
    uint8_t width;                              /*!< Width of the board */
    uint8_t height;                             /*!< Height of the board */
    uint16_t* board;                            /*!< Flattened three-dimensional array of
                                                     size (height + 2) x (width + 2) x 3 */
    expansion_list_t* exp_list;
} Board_t;

/**
 * \brief       Represents the settings needed to randomize the board.
 */
typedef struct RandomizeSettings {
    uint8_t m;                                  /*!< Some setting for randomizing
                                                     the board */
} RandomizeSettings_t;

#endif

