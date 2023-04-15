#ifndef LOAD_TILE_INFO_H
#define LOAD_TILE_INFO_H

#include <stdlib.h>
#include <stdio.h>
#include "data_utils.h"

#include "../utils/linked_list.h"
#include "../utils/hash_map.h"

#define MAX_ID_HASH         (100)
#define MAX_CONNECTION_HASH (1000)

tile_info_t* load_tile_info(char*, hash_map_t**, hash_map_t**);
tile_info_t* allocate_tile_info(linked_list_t*, uint8_t, uint8_t);
size_t       parse_name(size_t, char*, char*);

tile_info_t* load_tile_info(char* tile_path, hash_map_t** tile_ids, hash_map_t** connections) {
    FILE* fptr;
    tile_info_t* tile_info;
    int state, id_amount, connection_amount, num, ind, i, j;
    char line[500], *file_name, name[5];
    linked_list_t* tile_networks;
    list_element_t* elm;

    *tile_ids = init_hash_map(23, MAX_ID_HASH);
    *connections = init_hash_map(18, MAX_CONNECTION_HASH);

    file_name = concat_string(tile_path, "tile_info");
    if ((fptr = fopen(file_name, "r")) == NULL) {
        printf("Fatal error: could not find tile data.\n");
        exit(1);
    }
    free(file_name);

    state = -1;
    tile_networks = init_list();
    while (fgets(line, sizeof line, fptr) != NULL) {
        switch (state) {
            case -1:
                if (string_starts_with("TILES", line)) {
                    state++;
                    id_amount = 0;
                    continue;
                }
                break;
            case 0:
                if (string_starts_with("CONNECTIONS", line)) {
                    state++;
                    connection_amount = 0;
                    continue;
                }

                parse_name(0, line, name);
                if (name[0] != '\0') {
                    add_num(*tile_ids, name, id_amount++);
                    append(tile_networks, parse_internal_networks(line));
                }

                break;
            case 1:
                if (string_starts_with("TRAVERSABLE", line)) {
                    state++;

                    tile_info = allocate_tile_info(tile_networks, id_amount, connection_amount);

                    elm = tile_networks->frst;
                    for (i = 0; i < tile_networks->size; i++) {
                        for (j = 0; j < 8; j++) {
                            tile_info->networks[8 * i + j] = ((bool*) elm->data)[j];
                        }
                        elm = elm->next;
                    }

                    free_list(tile_networks, free);
                    continue;
                }

                parse_name(0, line, name);
                if (name[0] != '\0') {
                    add_num(*connections, name, connection_amount++);
                }
                break;
            case 2:
                if (string_starts_with("NON_CONNECTIONS", line)) {
                    state++;
                    continue;
                }

                parse_name(0, line, name);
                if (name[0] != '\0') {
                    tile_info->traversable[get_num(*connections, name)] = true;
                }
                break;
            case 3:
                if (string_starts_with("VALID_CONNECTIONS", line)) {
                    state++;
                    continue;
                }

                parse_name(0, line, name);
                if (name[0] != '\0') {
                    tile_info->non_connection[get_num(*connections, name)] = true;
                }
                
                break;
            case 4:
                if (line[0] != '(') continue;

                j = parse_name(1, line, name);
                if (name[0] != '\0') {
                    num = get_num(*connections, name);
                }

                while (line[j] != ')') {
                    while (line[j] == ' ' || line[j] == ',') j++;

                    j = parse_name(j, line, name);
                    if (name[0] != '\0') {
                        ind = num * connection_amount + get_num(*connections, name);
                        tile_info->valid_connection[ind] = true;
                    }
                }
                break;
        }
    }

    DEBUG_PRINT(INFO, "Found %d different tile ids.\n", id_amount);
    DEBUG_PRINT(INFO, "Found %d different connection types.\n\n", connection_amount);
    
    DEBUG_PRINT(INFO, "Internal networks:%c\n", ' ');
    for (i = 0; i < id_amount; i++) {
        for (j = 0; j < 4; j++)
            DEBUG_PRINT(INFO, "%d", tile_info->networks[8 * i + j]);
        DEBUG_PRINT(INFO, "%c", ' ');
        for (j = 0; j < 4; j++)
            DEBUG_PRINT(INFO, "%d", tile_info->networks[8 * i + 4 + j]);
        DEBUG_PRINT(INFO, "%c\n", ' ');
    }
    DEBUG_PRINT(INFO, "%c\n", ' ');

    DEBUG_PRINT(INFO, "Traversable:%c\n", ' ');
    for (i = 0; i < connection_amount; i++) {
        DEBUG_PRINT(INFO, "%d", tile_info->traversable[i]);
    }
    DEBUG_PRINT(INFO, "%c\n", ' ');
    DEBUG_PRINT(INFO, "%c\n", ' ');

    DEBUG_PRINT(INFO, "Non connections:%c\n", ' ');
    for (i = 0; i < connection_amount; i++) {
        DEBUG_PRINT(INFO, "%d", tile_info->non_connection[i]);
    }
    DEBUG_PRINT(INFO, "%c\n", ' ');
    DEBUG_PRINT(INFO, "%c\n", ' ');

    DEBUG_PRINT(INFO, "Valid connections:%c\n", ' ');
    for (i = 0; i < connection_amount; i++) {
        DEBUG_PRINT(INFO, "%d: ", i);
        for (j = 0; j < connection_amount; j++) {
            DEBUG_PRINT(INFO, "%d", tile_info->valid_connection[i * connection_amount + j]);
        }
        DEBUG_PRINT(INFO, "%c\n", ' ');
    }
    DEBUG_PRINT(INFO, "%c\n", ' ');

    fclose(fptr);
    return tile_info;
}

tile_info_t* allocate_tile_info(linked_list_t* tile_networks, uint8_t id_amount, uint8_t connection_amount) {
    size_t size;
    bool* data;
    tile_info_t* tile_info;
    
    tile_info = (tile_info_t*) malloc(sizeof(tile_info_t));
    tile_info->id_amount = id_amount;
    tile_info->connection_amount = connection_amount;
    
    size = 8 * tile_networks->size;
    size += 2 * connection_amount;
    size += connection_amount * connection_amount;

    data = (bool*) calloc(size, sizeof(bool));
    tile_info->networks = data;

    data += 8 * tile_networks->size;
    tile_info->traversable = data;

    data += connection_amount;
    tile_info->non_connection = data;

    data += connection_amount;
    tile_info->valid_connection = data;

    return tile_info;
}

void free_tile_info(tile_info_t* tile_info) {
    if (tile_info == NULL) return;

    free(tile_info->networks);
    free(tile_info);
}

size_t parse_name(size_t j, char* line, char* name) {
    int i = 0;
    while (line[j] != ' ' && line[j] != ')' && line[j] != ':' && line[j] != '\n' && line[j] != '\0' && line[j] != '#' && line[j] != ',' && i < 5) {
        if (line[j] == '\0') {
            i = 5;
            break;
        }
        name[i++] = line[j++];
    }
    name[i * (i < 5)] = '\0';
    return j;
}

#endif
