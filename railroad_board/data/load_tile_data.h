#ifndef LOAD_TILE_DATA_H
#define LOAD_TILE_DATA_H

#include "stdlib.h"
#include "stdio.h"

#include "data_utils.h"
#include "../utils/utils.h"
#include "../utils/linked_list.h"
#include "../utils/hash_map.h"
#include "../utils/debug_utils.h"

tile_data_t*   load_tiles(char*, uint8_t, char**, hash_map_t*, hash_map_t*, uint16_t*, hash_map_t**, uint8_t**);
tile_data_t*   assemble_tile_data(uint8_t, linked_list_t**);
tile_data_t*   allocate_tile_data(uint8_t, size_t);
linked_list_t* load_expansion_tiles(char*, char*, hash_map_t*, hash_map_t*, uint16_t*, hash_map_t*, uint8_t*);
single_tile_t* parse_single_tile(char*, hash_map_t*, hash_map_t*, hash_map_t*);
uint16_t       load_types(FILE*, uint16_t*, hash_map_t*);

tile_data_t* load_tiles(char* tile_path, uint8_t amount, char** expansion_name, hash_map_t* tile_ids, hash_map_t* connections, uint16_t* type_amount, hash_map_t** types, uint8_t** types_in_expansion) {
    int i;
    tile_data_t* tiles;
    linked_list_t** list;

    list = calloc(amount, sizeof(linked_list_t*));
    *types = init_hash_map(1, 1000);
    *types_in_expansion = calloc(amount, sizeof(uint8_t));
    
    for (i = 0; i < amount; i++) {
        list[i] = load_expansion_tiles(tile_path, expansion_name[i], tile_ids, connections, type_amount, *types, &(*types_in_expansion)[i]);
    }
    
    tiles = assemble_tile_data(amount, list);

    for (i = 0; i < amount; i++) free_list(list[i], free);
    free(list);

    for (i = 0; i < tiles->expansion_ind[tiles->expansion_amount - 1]; i++) {
        DEBUG_PRINT(DEBUG, "%2d %d: ", i, tiles->ids[i]);
        DEBUG_PRINT(DEBUG, "%d (", tiles->types[i]);
        for (int j = 0; j < 4; j++) {
            if (j > 0) DEBUG_PRINT(DEBUG, "%c", ' ');
            DEBUG_PRINT(DEBUG, "%d", tiles->connections[4 * i + j]);
        }
        DEBUG_PRINT(DEBUG, ") %d\n", tiles->stations[i]);
    }

    DEBUG_PRINT(INFO, "\nLoaded %d types and %d tiles from %d expansions.\n", *type_amount, tiles->expansion_ind[tiles->expansion_amount - 1], tiles->expansion_amount);

    return tiles;
}

tile_data_t* assemble_tile_data(uint8_t expansion_amount, linked_list_t** list) {
    size_t i, j, k, ind, tile_amount;
    list_element_t* elm;
    single_tile_t* tile;
    tile_data_t* tile_data;

    tile_amount = 0;
    for (i = 0; i < expansion_amount; i++) {
        tile_amount += list[i]->size;
    }

    tile_data = allocate_tile_data(expansion_amount, tile_amount);

    ind = 0;
    for (i = 0; i < expansion_amount; i++) {
        elm = list[i]->frst;
        for (j = 0; j < list[i]->size; j++) {
            tile = elm->data;

            tile_data->ids[ind] = tile->id;
            tile_data->types[ind] = tile->type;
            for (k = 0; k < 4; k++) {
                tile_data->connections[4 * ind + k] = tile->connections[k];
            }
            tile_data->stations[ind] = tile->station;

            elm = elm->next;
            ind += 1;
        }
        tile_data->expansion_ind[i] = ind;
    }

    return tile_data;
}

linked_list_t* load_expansion_tiles(char* tile_path, char* expansion_name, hash_map_t* tile_ids, hash_map_t* connections, uint16_t* type_amount, hash_map_t* types, uint8_t* types_in_expansion) {
    FILE* fptr;
    single_tile_t* tile;
    linked_list_t* list;
    int i, j;
    char c, line[200], *file_name;
    
    file_name = concat_string(tile_path, expansion_name);
    list = init_list();
    
    if ((fptr = fopen(file_name, "r")) == NULL) {
        printf("Fatal error: Could not load expansion %s.\n", expansion_name);
        exit(1);
    }
    free(file_name);
    
    if (fgets(line, sizeof line, fptr) == NULL) {
        return list;
    }

    *types_in_expansion = 0;
    if (string_starts_with("EXTRA:", line)) {
        *types_in_expansion = load_types(fptr, type_amount, types);
    }

    while (fgets(line, sizeof line, fptr) != NULL) {
        if (line[0] != '\n' && line[0] != '#' && line[0] != ' ') {
            append(list, parse_single_tile(line, tile_ids, connections, types));
        }
    }
    
    fclose(fptr);
    
    DEBUG_PRINT(INFO, "Loaded %d types and %lu tiles from %s.\n", *types_in_expansion, list->size, expansion_name);
    return list;
}

single_tile_t* parse_single_tile(char* line, hash_map_t* tile_ids, hash_map_t* connections, hash_map_t* types) {
    int i, j, k;
    char name[5];
    single_tile_t* tile;
    tile = malloc(sizeof(single_tile_t));

    i = 0; j = 0;
    while (line[j] != ':' && j < 5) {
        name[i++] = line[j++];
    }
    name[i * (i < 5)] = '\0';
    tile->id = get_num(tile_ids, name);
    DEBUG_PRINT(DEBUG, "%s: ", name);

    while (line[j] == ' ' || line[j] == ':') j++;

    i = 0;
    while (line[j] != ' ' && j < 5) {
        name[i++] = line[j++];
    }
    name[i * (i < 5)] = '\0';
    tile->type = get_num(types, name);
    DEBUG_PRINT(DEBUG, "%s (", name);

    while (line[j++] != '(');
    for (k = 0; k < 4; k++) {
        while (line[j] == ' ') j++;
        i = 0;
        while (line[j] != ' ' && line[j] != ')' && i < 5) {
            name[i++] = line[j++];
        }
        name[i * (i < 5)] = '\0';
        
        tile->connections[k] = get_num(connections, name);

        if (k > 0) DEBUG_PRINT(DEBUG, "%c", ' ');
        DEBUG_PRINT(DEBUG, "%s", name);
    }
    DEBUG_PRINT(DEBUG, ")%c", ' ');

    while (line[j] == ' ' || line[j] == ')') j++;
    if (line[j] == 'S') {
        tile->station = true;
    } else {
        tile->station = false;
    }

    DEBUG_PRINT(DEBUG, "=> %d: %d ", tile->id, tile->type);
    DEBUG_PRINT(DEBUG, "(%d %d %d %d) ", tile->connections[0], tile->connections[1], tile->connections[2], tile->connections[3]);
    DEBUG_PRINT(DEBUG, "%d\n", tile->station);

    return tile;
}

uint16_t load_types(FILE* fptr, uint16_t* type_amount, hash_map_t* types) {
    int i;
    uint16_t types_in_expansion;
    char line[200], name[5];

    types_in_expansion = 0;
    while ((fgets(line, sizeof line, fptr)) != NULL) {
        i = 0;
        while (line[i] != ' ' && line[i] != ';' && line[i] != '\n' && line[i] != '\0' && line[i] != '#' && i < 5) {
            name[i] = line[i];
            i++;
        }
        name[i * (i < 5)] = '\0';
        if (name[0] != '\0') {
            types_in_expansion += 1;
            add_num(types, name, (*type_amount)++);
        }

        if (line[i] == ';') break;
    }

    return types_in_expansion;
}

tile_data_t* allocate_tile_data(uint8_t expansion_amount, size_t tile_amount) {
    size_t size;
    void* data;
    tile_data_t* tile_data;

    size = sizeof(tile_data_t);
    size += expansion_amount * sizeof(uint16_t);
    size += tile_amount * sizeof(uint8_t);
    size += tile_amount * sizeof(uint8_t);
    size += 4 * tile_amount * sizeof(uint8_t);
    size += tile_amount * sizeof(bool);

    data = malloc(size);

    tile_data = data;
    tile_data->expansion_amount = expansion_amount;

    data += sizeof(tile_data_t);
    tile_data->expansion_ind = data;

    data += expansion_amount * sizeof(uint16_t);
    tile_data->ids = data;

    data += tile_amount * sizeof(uint8_t);
    tile_data->types = data;
    
    data += tile_amount * sizeof(uint8_t);
    tile_data->connections = data;

    data += 4 * tile_amount * sizeof(uint8_t);
    tile_data->stations = data;

    return tile_data;
}

#endif
