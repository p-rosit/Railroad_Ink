#ifndef LOAD_TILE_META_DATA_H
#define LOAD_TILE_META_DATA_H

#include "../utils/utils.h"
#include "data_utils.h"

void load_tile_meta_data(game_data_t*);

void load_tile_meta_data(game_data_t* game_data) {
    FILE* fptr;
    char identifier[IDENTIFIER_SIZE];
    string_t path;

    path = join_path(2, game_data->data_path, "meta_data");

    printf("(%s)\n", path);
    free(path);
}

void free_tile_meta_data(game_data_t* game_data) {
    /*
    expansions_t* expansions = game_data->expansions;

    if (settings != NULL) {
    
    }

    if (expansions != NULL) {
    
    }

    free(settings);
    free(expansions);
    */
}

#endif
