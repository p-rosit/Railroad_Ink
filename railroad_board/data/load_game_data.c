#ifndef LOAD_GAME_DATA_H
#define LOAD_GAME_DATA_H

#include "../utils/utils.c"
#include "../utils/string_map.c"
#include "../utils/robin_hood.c"
#include "data_utils.c"

#include "load_meta_data.c"
#include "load_tile_meta_data.c"
#include "load_expansion_data.c"
#include "prepare_types.c"
#include "prepare_tiles.c"
#include <stdlib.h>

game_data_t*    load_game_data(string);
void            free_game_data(game_data_t*);
string_map_t*   convert_types(linked_list_t*);

game_data_t* load_game_data(string directory_name) {
    linked_list_t* list;
    list_element_t* elm;
    game_data_t* game_data;
    temp_meta_data_t* tmd;
    temp_tile_data_t* ttd;
    temp_expansion_data_t* ted;

    game_data = malloc(sizeof(game_data_t));
    game_data->map = malloc(sizeof(name_mapping_t));
    game_data->map->dice = init_robin_hash(10, 1000);
    game_data->settings = malloc(sizeof(settings_t));
    game_data->settings->data_path = copy_str(directory_name);

    ttd = load_tile_meta_data(game_data);
    
    game_data->map->tile = make_string_map_u8(10, 1000, ttd->tile_ids);
    game_data->map->connection = make_string_map_u8(10, 1000, ttd->tile_connections);

    tmd = load_meta_data(game_data);
    ted = init_temp_expansion_data();
    ted->total_types = 1;

    list = init_list();
    append(list, copy_str(tmd->empty_type));
    append(ted->types, list);

    for (elm = tmd->expansion_files->frst; elm != NULL; elm = elm->next) {
        load_expansion_data(elm->data, game_data, ted);
    }

    game_data->map->type = convert_types(ted->types);

    /* Prepare data */
    linked_list_t* ll;
    printf("Total types: %d\n", ted->total_types);
    for (elm = ted->types->frst; elm != NULL; elm = elm->next) {
        ll = elm->data;
        for (list_element_t* nelm = ll->frst; nelm != NULL; nelm = nelm->next) {
            printf("%s: %d\n", (string) nelm->data, get_mapped_u8(game_data->map->type, nelm->data));
        }
    }

    prepare_types(game_data, ted);
    prepare_tiles(game_data, ted);

    free_temp_meta_data(tmd);
    free_temp_tile_data(ttd);
    free_temp_expansion_data(ted);
    return game_data;
}

string_map_t* convert_types(linked_list_t* types) {
    linked_list_t *list, *flat_types;
    list_element_t *elm, *temp;
    string_map_t* map;

    flat_types = init_list();

    for (elm = types->frst; elm != NULL; elm = elm->next) {
        list = elm->data;
        for (temp = list->frst; temp != NULL; temp = temp->next) {
            append(flat_types, temp->data);
        }
    }
    
    map = make_string_map_u8(10, 1000, flat_types);
    free_list(flat_types, NULL);

    return map;
}

void free_game_data(game_data_t* game_data) {
    if (game_data == NULL) return;

    free_name_mapping(game_data->map);
    free_tile_meta_data(game_data);

    free_settings(game_data->settings);
    free_type_data(game_data->types);
    free_tile_data(game_data->tiles);
    free(game_data);
}

#endif
