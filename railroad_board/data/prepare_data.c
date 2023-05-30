#ifndef PREPARE_DATA_H
#define PREPARE_DATA_H

#include "../utils/linked_list.c"
#include "data_utils.c"
#include <stdint.h>
#include <stdlib.h>

void                prepare_types(game_data_t*, temp_expansion_data_t*);
uint16_t            count_type_combinations(settings_t*, linked_list_t*);
uint16_t            count_combinations_of_n_types(uint16_t, uint16_t*);
void                make_type_combinations(settings_t*, type_data_t*, linked_list_t*);
void                make_combinations_of_n_types(type_data_t*, uint16_t, uint16_t*);
void                free_type_data(type_data_t*);

void prepare_tiles(game_data_t*);
void free_tile_data(tile_data_t*);

void prepare_types(game_data_t* game_data, temp_expansion_data_t* ted) {
    printf("Preparing\n");
    uint8_t combinations;
    settings_t* settings;

    settings = game_data->settings;
    game_data->types = malloc(sizeof(type_data_t));

    combinations = count_type_combinations(settings, ted->types);
    make_type_combinations(settings, game_data->types, ted->types);
}

uint16_t count_type_combinations(settings_t* settings, linked_list_t* types) {
    int i, j;
    uint16_t ind, type_ind;

    list_element_t *elm, *list_elm;
    linked_list_t* list;
    for (elm = types->frst; elm != NULL; elm = elm->next) {
        list = elm->data;
        printf("%lu: ", list->size);
        for (list_elm = list->frst; list_elm != NULL; list_elm = list_elm->next) {
            printf("%s ", (string) list_elm->data);
        }
        printf("\n");
    }

    ind = 1;
    type_ind = 1;
    for (i = 0; i < settings->max_combinations; i++) {
        for (j = 0; j < types->size; j++) {
            ind = count_combinations_of_n_types(ind, &type_ind);
        }
    }
    return 0;
}

uint16_t count_combinations_of_n_types(uint16_t ind, uint16_t* type_ind) {
    return ind;
}

void make_type_combinations(settings_t* settings, type_data_t* type_data, linked_list_t* types) {
    int i, j;
    uint16_t ind, type_ind;
    
    ind = 1;
    type_ind = 1;
    for (i = 0; i < settings->max_combinations; i++) {
        for (j = 0; j < types->size; j++) {
            make_combinations_of_n_types(type_data, ind, &type_ind);
        }
    }
}

void make_combinations_of_n_types(type_data_t* type_data, uint16_t ind, uint16_t* type_ind) {

}

void free_type_data(type_data_t* type_data) {
    if (type_data == NULL) return;

    free(type_data);
}

void prepare_tiles(game_data_t* game_data) {
    game_data->tiles = malloc(sizeof(tile_data_t));
}

void free_tile_data(tile_data_t* tile_data) {
    if (tile_data == NULL) return;

    free(tile_data);
}

#endif
