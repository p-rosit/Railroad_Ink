#ifndef LOAD_GAME_DATA_H
#define LOAD_GAME_DATA_H

#include "../utils/utils.c"
#include "../utils/string_map.c"
#include "../utils/robin_hood.c"
#include "data_utils.c"

#include "load_meta_data.c"
#include "load_tile_meta_data.c"
#include "load_expansion_data.c"
#include "prepare_data.c"
#include <stdlib.h>

game_data_t*    load_game_data(string);
void            free_game_data(game_data_t*);

game_data_t* load_game_data(string directory_name) {
    list_element_t* elm;
    game_data_t* game_data;
    temp_meta_data_t* tmd;
    temp_tile_data_t* ttd;
    temp_expansion_data_t* ted;
    string_map_t* map;

    game_data = malloc(sizeof(game_data_t));
    game_data->map = malloc(sizeof(name_mapping_t));
    game_data->map->dice = init_robin_hash(10, 1000);
    game_data->map->type = init_robin_hash(10, 1000);
    game_data->data_path = copy_str(directory_name);

    ttd = load_tile_meta_data(game_data);
    
    game_data->map->tile = make_string_map_u8(10, 1000, ttd->tile_ids);
    game_data->map->connection = make_string_map_u8(10, 1000, ttd->tile_connections);

    tmd = load_meta_data(game_data);
    ted = init_temp_expansion_data();

    for (elm = tmd->expansion_files->frst; elm != NULL; elm = elm->next) {
        load_expansion_data(elm->data, game_data, ted);
    }

    /* Prepare data */
    printf("Total types: %d\n", ted->total_types);
    for (elm = ted->types->frst; elm != NULL; elm = elm->next) {
        printf("ttttt\n");
        for (list_element_t* nelm = ((linked_list_t*) elm)->frst; nelm != NULL; nelm = nelm->next) {
            printf("(%s)\n", (string) nelm->data);
            //printf("%s: %d\n", (string) nelm->data, get_val_u16(game_data->map->type, hash_string(nelm->data)));
        }
    }

    free_temp_meta_data(tmd);
    free_temp_tile_data(ttd);
    free_temp_expansion_data(ted);
    return game_data;
}

void free_game_data(game_data_t* game_data) {
    if (game_data == NULL) return;

    free_meta_data(game_data);
    free_name_mapping(game_data);
    free_tile_meta_data(game_data);

    free(game_data->data_path);
    free(game_data);
}

#endif
