#include <stdio.h>

#include "board_struct.h"

/* Functions and structs which are implemented */
#include "../railroad_board.h"


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

