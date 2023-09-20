#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "../../railroad_constants.h"
#include "../../railroad_types.h"
#include "../../data/railroad_data.h"
#include "board_struct.h"

/* Functions and structs which are implemented */
#include "../railroad_board.h"


void free_board(board_t board) {
    free((board_data_t *) board->game_data.tile_data);
    free(board->tiles);
    free(board);
}

bool coord_on_board(board_t board, board_coord_t coord) {
    bool in_cols = (1 <= coord.col) && (coord.col <= board->height);
    bool in_rows = (1 <= coord.row) && (coord.row <= board->width);
    return in_cols && in_cols;
}

void add_tile_to_board(board_t board, tile_t tile, board_coord_t coord) {
    memcpy(&board->tiles[coord.row * (board->width + 2) + coord.col], &tile, sizeof(tile_t));
}

void set_tile_data(board_t board, board_data_t value, size_t index, board_coord_t coord) {
    board->tiles[coord.row * (board->width + 2) + coord.col].data[index] = value;
}

void print_board(board_t board, size_t layer) {
    bool is_border, js_border;
    int size;
    board_size_t i, j;
    tile_t tile;
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

void print_game_data(board_t board) {
    for (size_t i = 0; i < MAX_EXPANSIONS && board->expansions[i] != NO_EXPANSION; i++) {
        printf("%lu, ", board->expansions[i]);
    }
    printf("\n");

    printf(":) (%lu, %lu)\n", board->height, board->width);
}


