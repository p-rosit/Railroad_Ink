#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include "../railroad_board.h"
#include "board_struct.h"

bool    save_expansions(board_load_info_t, board_t);
size_t  mark_tile_index(board_t);
size_t  mark_type_index(board_t);
void    load_board_data(board_t);


board_t make_board(board_load_info_t info) {
    size_t total_entries, marked;
    board_t board;
    bool expansions_ok;

    total_entries = (info.height + 2) * (info.width * 2);

    board = malloc(sizeof(struct board));
    board->width = info.width;
    board->height = info.height;
    board->tiles = calloc(total_entries, sizeof(tile_t));

    expansions_ok = save_expansions(info, board);

    if (!expansions_ok) {
        return NULL;
    }

    marked = mark_tile_index(board);
    marked = mark_type_index(board);

    if (marked > MAX_TYPE_EXPANSIONS) {
        // printf("WARNING: Tried to load more than %lu expansions with types.", marked);
        return NULL;
    }

    load_board_data(board);

    return board;
}

bool save_expansions(board_load_info_t info, board_t board) {
    size_t i, curr;
    expansion_index_t index;
    bool expansions_ok = true;

    for (i = 0, curr = 0; i < MAX_TYPE_EXPANSIONS; i++) {
        index = info.expansions[i];

        if (!(index < MAX_TYPE_EXPANSIONS) && index != NO_EXPANSION) {
            // printf("WARNING: Trying to load unknown expansions %lu.", index);
            expansions_ok = false;
            continue;
        } 

        if (index != NO_EXPANSION) {
            board->expansions[curr++] = index;
        }
    }

    return expansions_ok;
}

size_t mark_tile_index(board_t board) {
    size_t i, total;
    bool has_tiles;

    for (i = 0, total = 0; i < MAX_EXPANSIONS; i++) {
        has_tiles = expansion_has_tiles(board->expansions[i]);
        board->expansion_tile_index[i] = has_tiles * i + !has_tiles * MAX_EXPANSIONS;
        total += has_tiles;
    }

    return total;
}

size_t mark_type_index(board_t board) {
    size_t i, total;
    bool has_types;

    for (i = 0, total = 0; i < MAX_EXPANSIONS; i++) {
        has_types = expansion_has_types(board->expansions[i]);
        board->expansion_type_index[i] = has_types * i + !has_types * MAX_EXPANSIONS;
        total += has_types;
    }

    return total;
}

void load_board_data(board_t board) {
    /* Bypass C type system to assign to const once */
    game_data_t game_data = load_data(board->expansions);
    memcpy((game_data_t*) &(board->game_data), &game_data, sizeof(game_data_t));
}

void free_board(board_t board) {
    // free((board_data_t *) (board->game_data.tile_data));
    free(board->tiles);
    free(board);
}

