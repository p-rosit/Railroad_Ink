#ifndef LOAD_GAME_DATA_H
#define LOAD_GAME_DATA_H

#include "../utils/utils.c"
#include "data_utils.c"

#include "load_meta_data.c"
#include "load_tile_meta_data.c"

game_data_t*    load_game_data(string);
void            free_game_data(game_data_t*);

game_data_t* load_game_data(string directory_name) {
    game_data_t* game_data;
    temp_meta_data_t* tmd;
    temp_tile_data_t* ttd;

    game_data = (game_data_t*) malloc(sizeof(game_data_t));
    game_data->data_path = copy_str(directory_name);

    ttd = load_tile_meta_data(game_data);
    tmd = load_meta_data(game_data);
    
    free_temp_meta_data(tmd);
    free_temp_tile_data(ttd);
    return game_data;
}

void free_game_data(game_data_t* game_data) {
    if (game_data == NULL) return;

    free_meta_data(game_data);
    free_tile_meta_data(game_data);

    free(game_data->data_path);
    free(game_data);
}

#endif
