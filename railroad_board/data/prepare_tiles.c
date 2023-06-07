#ifndef PREPARE_TILES_H
#define PREPARE_TILES_H

#include "../utils/linked_list.c"
#include "data_utils.c"
#include <stdint.h>
#include <stdlib.h>

void prepare_tiles(game_data_t*, temp_expansion_data_t*);
void free_tile_data(tile_data_t*);

void prepare_tiles(game_data_t* game_data, temp_expansion_data_t* ted) {
    int i;
    uint8_t* tiles;
    list_element_t* elm;
    temp_tile_t* tile;
    tile_data_t* tile_data;

    tile_data = malloc(sizeof(tile_data_t));
    tile_data->total_tiles = ted->tiles->size;
    tile_data->tiles = malloc(6 * tile_data->total_tiles * sizeof(uint8_t));
    tiles = tile_data->tiles;

    for (elm = ted->tiles->frst->next; elm != NULL; elm = elm->next) {
        tile = elm->data;
        
        tiles[0] = get_mapped_u8(game_data->map->tile, tile->id);

        for (i = 0; i < 4; i++) {
            tiles[i + 1] = get_mapped_u8(game_data->map->connection, tile->connections[i]);
        }

        tiles[5] = tile->station[0] + 2 * tile->station[1];

        tiles += 6;
    }

    game_data->tiles = tile_data;
}

void free_tile_data(tile_data_t* tile_data) {
    if (tile_data == NULL) return;

    free(tile_data->tiles);
    free(tile_data);
}

#endif
