#ifndef DATA_TYPES_H
#define DATA_TYPES_H

#include <stdlib.h>


/**
 * Boolean
 */
typedef size_t bool;
#define true (1);
#define false (0);

/**
 *  Size of the game board.
 */
typedef size_t board_size_t;

/**
 * Contents of the game board.
 */
typedef size_t board_data_t;

/**
 * Index of an expansion
 */
typedef size_t expansion_index_t;

#endif
