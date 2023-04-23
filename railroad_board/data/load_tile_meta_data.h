#ifndef LOAD_TILE_META_DATA_H
#define LOAD_TILE_META_DATA_H

#include "data_utils.h"

void load_tile_meta_data(game_data_t*);

void load_tile_meta_data(game_data_t* game_data) {

}

void free_tile_meta_data(game_data_t* game_data) {
    settings_t* settings = game_data->settings;
    expansions_t* expansions = game_data->expansions;

    if (settings != NULL) {
    
    }

    if (expansions != NULL) {
    
    }

    free(settings);
    free(expansions);
}

#endif
