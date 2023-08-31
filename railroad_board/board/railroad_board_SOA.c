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

    /* SOA board data*/
    board_data_t* expansion_index;
    board_data_t* local_index;
    board_data_t* orientation;
    board_data_t* center;
    board_data_t* data;
    board_data_t* connections;
    board_data_t* networks;
};

Board_t make_board(board_size_t height, board_size_t width) {
    size_t board_depth, total_entries;
    board_data_t* data_ptr;
    Board_t board;

    board_depth = 4 + MAX_EXPANSIONS + 12;
    total_entries = (height + 2) * (width + 2);

    board = malloc(sizeof(struct Board));
    data_ptr = calloc(total_entries * board_depth, sizeof(board_data_t));

    board->height = height;
    board->width = width;
    board->expansion_index = data_ptr;   data_ptr += total_entries;
    board->local_index = data_ptr;       data_ptr += total_entries;
    board->orientation = data_ptr;       data_ptr += total_entries;
    board->center = data_ptr;            data_ptr += total_entries;
    board->data = data_ptr;              data_ptr += MAX_EXPANSIONS * total_entries;
    board->connections = data_ptr;       data_ptr += 4 * total_entries;
    board->networks = data_ptr;          data_ptr += 8 * total_entries;

    return board;
}

void free_board(Board_t board) {
    free(board->expansion_index);
    free(board);
}

bool coord_on_board(Board_t board, size_t i, size_t j) {
    return (1 <= i) && (i <= board->height) && (1 <= j) && (j <= board->width);
}

void add_tile_to_board(Board_t board, Tile_t tile, size_t i, size_t j) {
    board_size_t index;

    index = i * (board->width + 2) + j;
    board->expansion_index[index] = tile.expansion_index;
    board->local_index[index] = tile.local_index;
    board->orientation[index] = tile.orientation;
    board->center[index] = tile.center;

    index = i * board->width * MAX_EXPANSIONS + j;
    memcpy(&board->data[index], &tile.data, MAX_EXPANSIONS * sizeof(board_data_t));
    index = i * board->width * 4 + j;
    memcpy(&board->connections[index], &tile.connections, 4 * sizeof(board_data_t));
    index = i * board->width * 8 + j;
    memcpy(&board->networks[index], &tile.networks, 8 * sizeof(board_data_t));
}

void set_tile_data(Board_t board, board_data_t value, size_t index, size_t i, size_t j) {
    board->data[i * (board->width + 2) * MAX_EXPANSIONS + j * MAX_EXPANSIONS + index] = value;
}

void print_board(Board_t board, size_t layer) {
    bool is_border, js_border;
    board_size_t i, j, k, index;
    board_data_t local_index;

    for (i = 0; i < board->height + 2; i++) {
        is_border = i * (i - board->height - 1) == 0;
        for (j = 0; j < board->width + 2; j++) {
            js_border = is_border || j * (j - board->width - 1) == 0;
            index = i * (board->width + 2) + j;
            local_index = board->local_index[index];

            if (local_index != 0 || !js_border) {
                switch (layer) {
                    case 0:
                        printf("%2lu ", board->expansion_index[index]); break;
                    case 1:
                        printf("%2lu ", board->local_index[index]); break;
                    case 2:
                        printf("%2lu ", board->orientation[index]); break;
                    case 3:
                        printf("%2lu ", board->center[index]); break;
                }
                if (3 < layer && layer < 3 + MAX_EXPANSIONS + 1) {
                    index = i * (board->width + 2) * MAX_EXPANSIONS + j * MAX_EXPANSIONS + (layer - 3);
                    printf("%2lu ", board->data[index]);
                }
                if (3 + MAX_EXPANSIONS + 1 < layer && layer < 3 + MAX_EXPANSIONS + 1 + 4 + 1) {
                    index = i * (board->width + 2) * MAX_EXPANSIONS + j * MAX_EXPANSIONS + (layer - 3 - MAX_EXPANSIONS);
                    printf("%2lu ", board->connections[index]);
                }
            } else {
                printf("   ");
            }
        }
        printf("\n");
    }
}

