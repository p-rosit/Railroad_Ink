#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include "../railroad_board.h"
#include "board_struct.h"

bool    save_expansions(board_load_info_t, board_t);
size_t  mark_tile_expansions(board_t);
size_t  mark_type_expansions(board_t);
void    load_board_data(board_t);

/* Implementation of railroad_board.h */

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
        /* Tried to load unknown expansion. */
        return NULL;
    }

    marked = mark_tile_expansions(board);
    marked = mark_type_expansions(board);

    if (marked > MAX_TYPE_EXPANSIONS) {
        /* Tried to load more than MAX_TYPE_EXPANSIONS expansions with types. */
        return NULL;
    }

    load_board_data(board);

    return board;
}


void free_board(board_t board) {
    if (board == NULL) return;
    free((board_data_t *) (board->game_data.tile_data));
    free(board->tiles);
    free(board);
}


/* Helper functions */

bool save_expansions(board_load_info_t info, board_t board) {
    size_t i, curr;
    expansion_index_t index;
    bool expansions_ok = true;

    for (i = 0, curr = 0; i < MAX_EXPANSIONS; i++) {
        index = info.expansions[i];

        if (!(index < TOTAL_EXPANSIONS) && index != NO_EXPANSION) {
            /* Trying to load unknown expansion. */
            expansions_ok = false;
            break;
        } 

        if (index != NO_EXPANSION) {
            board->expansions[curr++] = index;
        }
    }

    for (i = curr; i < MAX_EXPANSIONS; i++) {
        board->expansions[i] = NO_EXPANSION;
    }

    return expansions_ok;
}

size_t mark_tile_expansions(board_t board) {
    size_t i, total;
    expansion_index_t exp;
    bool has_tiles;

    for (i = 0, total = 0; i < MAX_EXPANSIONS; i++) {
        exp = board->expansions[i];
        has_tiles = expansion_has_tiles(exp);
        board->tile_expansions[i] = has_tiles * exp + !has_tiles * NO_EXPANSION;
        total += has_tiles;
    }

    return total;
}

size_t mark_type_expansions(board_t board) {
    size_t i, total;
    expansion_index_t exp;
    bool has_types;

    for (i = 0, total = 0; i < MAX_EXPANSIONS; i++) {
        exp = board->expansions[i];
        has_types = expansion_has_types(exp);
        board->type_expansions[i] = has_types * exp + !has_types * NO_EXPANSION;
        total += has_types;
    }

    return total;
}

void load_board_data(board_t board) {
    /* Bypass C type system to assign to const once */
    game_data_t game_data = load_data(board->tile_expansions);
    memcpy((game_data_t*) &(board->game_data), &game_data, sizeof(game_data_t));
}

