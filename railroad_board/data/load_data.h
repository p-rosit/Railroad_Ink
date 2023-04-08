#ifndef LOAD_DATA_H
#define LOAD_DATA_H

#include <stdlib.h>
#include <stdio.h>

#include "../utils/utils.h"
#include "../utils/linked_list.h"
#include "../utils/hash_map.h"
#include "data_utils.h"
#include "../mechanics/expansion_utils.h"

#define MAX_EXP_HASH        (1000)
#define MAX_EXPS            (50)
#define MAX_ID_HASH         (100)
#define MAX_CONNECTION_HASH (1000)

char*          find_file_name(expansion_id_t);
void           load_info();
tile_data_t*   load_tiles(uint8_t, char**, hash_map_t*, hash_map_t*);
linked_list_t* load_expansion_tiles(char*);

char* tile_path = "./tiles";

void load_info(uint8_t* amount, char*** expansion_name, hash_map_t** map) {
    FILE* fptr;
    size_t i, j;
    char line[100], name[30], *file_name;
    char c;

    file_name = concat_string(tile_path, "expansions");
    if ((fptr = fopen(file_name, "r")) == NULL) {
        printf("Fatal error: could not find expansion data.\n");
        exit(1);
    }
    free(file_name);

    *map = init_hash_map(59, MAX_EXP_HASH);
    *expansion_name = malloc(MAX_EXPS * sizeof(char*));

    while (fgets(line, sizeof line, fptr) != NULL) {
        i = 0; j = 0;
        c = line[j];
        while (c != '\n' && c != '\0') {
            while (c == ' ') c = line[j++];
            if (c == '#') break;

            name[i++] = c;
            c = line[++j];
        }
        if (i == 0) continue;
        name[i] = '\0';
        (*expansion_name)[*amount] = copy_string(name);
        add_num(*map, name, (*amount)++);
    }

    fclose(fptr);
}

tile_info_t* load_tile_info(hash_map_t** tile_ids, hash_map_t** connections) {
    FILE* fptr;
    tile_info_t* tile_info;
    void* data;
    int state, id_amount, connection_amount, num, ind, i, j;
    char line[500], *file_name, name[5];
    linked_list_t* tile_networks;
    list_element_t* elm;

    *tile_ids = init_hash_map(1, MAX_ID_HASH);
    *connections = init_hash_map(1, MAX_CONNECTION_HASH);

    file_name = concat_string(tile_path, "tile_data");
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

                i = 0;
                while (line[i] != ':' && i < 5) {
                    if (line[i] == '\0') {
                        i = 5;
                        break;
                    }
                    name[i] = line[i];
                    i++;
                }
                name[i * (i < 4)] = '\0';
                if (name[0] != '\0') {
                    add_num(*tile_ids, name, id_amount++);

                    append(tile_networks, parse_internal_networks(line));
                }

                break;
            case 1:
                if (string_starts_with("TRAVERSABLE", line)) {
                    state++;
                    
                    data = malloc(sizeof(tile_info_t) + 2 * sizeof(uint8_t) + (8 * tile_networks->size + 2 * connection_amount + connection_amount * connection_amount) * sizeof(bool));

                    tile_info = data;
                    tile_info->id_amount = id_amount;
                    tile_info->connection_amount = connection_amount;
                    data += sizeof(tile_info_t) + 2 * sizeof(uint8_t);
                    for (i = 0; i < 8 * tile_networks->size + 2 * connection_amount + connection_amount * connection_amount; i++) {
                        ((bool*) data)[i] = false;
                    }

                    tile_info->networks = data;
                    data += 8 * tile_networks->size * sizeof(bool);
                    
                    tile_info->traversable = data;
                    data += connection_amount * sizeof(bool);

                    tile_info->non_connection = data;
                    data += connection_amount * sizeof(bool);

                    tile_info->valid_connection = data;

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

                i = 0;
                while (line[i] != ' ' && line[i] != '\n' && line[i] != '#' && i < 5) {
                    if (line[i] == '\0') {
                        i = 5;
                        break;
                    }
                    name[i] = line[i];
                    i++;
                }
                name[i * (i < 4)] = '\0';
                if (name[0] != '\0') {
                    add_num(*connections, name, connection_amount++);
                }
                break;
            case 2:
                if (string_starts_with("NON_CONNECTIONS", line)) {
                    state++;
                    continue;
                }

                i = 0;
                while (line[i] != ' ' && line[i] != '\n' && line[i] != '#' && i < 5) {
                    if (line[i] == '\0') {
                        i = 5;
                        break;
                    }
                    name[i] = line[i];
                    i++;
                }
                name[i * i < 4] = '\0';
                if (name[0] != '\0') {
                    tile_info->traversable[get_num(*connections, name)] = true;
                }
                break;
            case 3:
                if (string_starts_with("VALID_CONNECTIONS", line)) {
                    state++;
                    continue;
                }

                i = 0;
                while (line[i] != ' ' && line[i] != '\n' && line[i] != '#' && i < 5) {
                    if (line[i] == '\0') {
                        i = 5;
                        break;
                    }
                    name[i] = line[i];
                    i++;
                }
                name[i * (i < 4)] = '\0';
                if (name[0] != '\0') {
                    tile_info->non_connection[get_num(*connections, name)] = true;
                }
                
                break;
            case 4:
                if (line[0] != '(') continue;

                i = 0; j = 1;
                while (line[j] != ' ' && line[j] != ',' && line[i] != '#' && i < 5) {
                    if (line[j] == '\0') {
                        i = 5;
                        break;
                    }
                    name[i++] = line[j++];
                }
                name[i * (i < 4)] = '\0';
                if (name[0] != '\0') {
                    num = get_num(*connections, name);
                }

                while (line[j] != ')') {
                    while (line[j] == ' ' || line[j] == ',') j++;

                    i = 0;
                    while (line[j] != ' ' && line[j] != ')' && i < 5) {
                        if (line[j] == '\0') {
                            i = 5;
                            break;
                        }
                        name[i++] = line[j++];
                    }
                    name[i * (i < 4)] = '\0';
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
    
    DEBUG_PRINT(INFO, "Internal%snetworks:\n", " ");
    for (i = 0; i < id_amount; i++) {
        for (j = 0; j < 4; j++)
            DEBUG_PRINT(INFO, "%d", tile_info->networks[8 * i + j]);
        DEBUG_PRINT(INFO, "%s", " ");
        for (j = 0; j < 4; j++)
            DEBUG_PRINT(INFO, "%d", tile_info->networks[8 * i + 4 + j]);
        DEBUG_PRINT(INFO, "%s\n", " ");
    }
    DEBUG_PRINT(INFO, "%s\n", " ");

    DEBUG_PRINT(INFO, "Traversable:%s\n", " ");
    for (i = 0; i < connection_amount; i++) {
        DEBUG_PRINT(INFO, "%d", tile_info->traversable[i]);
    }
    DEBUG_PRINT(INFO, "%s\n", " ");
    DEBUG_PRINT(INFO, "%s\n", " ");

    DEBUG_PRINT(INFO, "Non connections:%s\n", " ");
    for (i = 0; i < connection_amount; i++) {
        DEBUG_PRINT(INFO, "%d", tile_info->non_connection[i]);
    }
    DEBUG_PRINT(INFO, "%s\n", " ");
    DEBUG_PRINT(INFO, "%s\n", " ");

    DEBUG_PRINT(INFO, "Valid connections:%s\n", " ");
    for (i = 0; i < connection_amount; i++) {
        DEBUG_PRINT(INFO, "%d: ", i);
        for (j = 0; j < connection_amount; j++) {
            DEBUG_PRINT(INFO, "%d", tile_info->valid_connection[i * connection_amount + j]);
        }
        DEBUG_PRINT(INFO, "%s\n", " ");
    }
    DEBUG_PRINT(INFO, "%s\n", " ");

    fclose(fptr);
    return tile_info;
}

tile_data_t* load_tiles(uint8_t amount, char** expansion_name, hash_map_t* tile_ids, hash_map_t* connections) {
    int i;
    tile_data_t* tiles;
    linked_list_t** list;

    list = calloc(amount, sizeof(linked_list_t*));

    for (i = 0; i < amount; i++) {
        list[i] = load_expansion_tiles(expansion_name[i]);
    }

    tiles = assemble_tile_data(amount, list);

    for (i = 0; i < amount; i++) free_list(list[i], free);
    free(list);

    return tiles;
}

linked_list_t* load_expansion_tiles(char* expansion_name) {
    FILE* fptr;
    single_tile_t* tile;
    linked_list_t* list;
    int i, j;
    char c, line[100];
    char* file_name;

    file_name = concat_string(tile_path, expansion_name);
    list = init_list();

    if ((fptr = fopen(file_name, "r")) == NULL) {
        printf("Fatal error: could not load expansion %s\n", expansion_name);
        exit(1);
    }
    free(file_name);

    while (fgets(line, sizeof line, fptr) != NULL) {
        printf("%s", line);
    }

    fclose(fptr);

    DEBUG_PRINT(INFO, "Loaded %d tiles from %s.\n", 0, expansion_name);
    return list;
}

#endif

