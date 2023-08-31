#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../railroad_constants.h"
#include "../railroad_types.h"
#include "../railroad_structs.h"

/* Functions and structs which are implemented */
#include "../railroad_board.h"


/**
 * The game board. Contains the following fields
 *
 *      expansions:     The expansions on the board.
 *      width:          The width of the board.
 *      height:         The height of the board.
 *      board:          A grid of tiles (width + 2) * (height + 2).
 */
struct Board {
    /* Included expansions and which index they have */
    size_t expansions[MAX_EXPANSIONS];

    /* Board and information about it */
    board_size_t        height;
    board_size_t        width;
    Tile_t*             tiles;
};


Board_t make_board(board_size_t height, board_size_t width) {
    size_t total_entries;
    Tile_t* tiles;
    Board_t board;

    total_entries = (height + 2) * (width * 2);

    board = malloc(sizeof(struct Board));
    tiles = calloc(total_entries, sizeof(Tile_t));

    board->width = width;
    board->height = height;
    board->tiles = tiles;

    return board;
}

void free_board(Board_t board) {
    free(board->tiles);
    free(board);
}

bool coord_on_board(Board_t board, size_t i, size_t j) {
    return (1 <= i) && (i <= board->height) && (1 <= j) && (j <= board->width);
}

void add_tile_to_board(Board_t board, Tile_t tile, size_t i, size_t j) {
    memcpy(&board->tiles[i * (board->width + 2) + j], &tile, sizeof(Tile_t));
}

void set_tile_data(Board_t board, board_data_t value, size_t index, size_t i, size_t j) {
    board->tiles[i * (board->width + 2) + j].data[index] = value;
}

void print_board(Board_t board, size_t layer) {
    bool is_border, js_border;
    int size;
    board_size_t i, j;
    Tile_t tile;
    board_data_t value;

    value = 0;
    size = 2;
    for (i = 0; i < board->height + 2; i++) {
        is_border = i * (i - board->height - 1) == 0;
        for (j = 0; j < board->width + 2; j++) {
            js_border = is_border || j * (j - board->width - 1) == 0;
            tile = board->tiles[i * (board->width + 2) + j];

            if (tile.local_index != 0 || !js_border) {
                if (layer == EXPANSION_INDEX_INDEX) {
                    value = tile.expansion_index;
                } else if (layer == LOCAL_INDEX_INDEX) {
                    value = tile.local_index;
                } else if (layer == ORIENTATION_INDEX) {
                    value = tile.orientation;
                } else if (layer == CENTER_INDEX) {
                    value = tile.center;
                } else if (DATA_INDEX <= layer && layer <= DATA_INDEX + MAX_EXPANSIONS) {
                    value = tile.data[layer - DATA_INDEX];
                } else if (CONNECTIONS_INDEX <= layer && layer <= CONNECTIONS_INDEX + 4) {
                    value = tile.connections[layer - CONNECTIONS_INDEX];
                } else if (NETWORKS_INDEX <= layer && layer <= NETWORKS_INDEX + 8) {
                    value = tile.networks[layer - NETWORKS_INDEX];
                }

                printf("%*lu ", size, value);

            } else {
                printf("%*s ", size, "");
            }
        }
        printf("\n");
    }
}

