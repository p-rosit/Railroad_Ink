#ifndef LOAD_EXPANSION_DATA_H
#define LOAD_EXPANSION_DATA_H

#include <stdio.h>
#include <stdlib.h>
#define TYPE_SCOPE (1)
#define TILE_SCOPE (2)
#define DICE_SCOPE (3)

#include "../utils/utils.c"
#include "../utils/linked_list.c"
#include "data_utils.c"

typedef struct internal_expansion_data      internal_expansion_data_t;

struct internal_expansion_data {
    string identifier;
    string expansion_name;
    robin_hash_t* identifier2index;
};

temp_expansion_data_t*      init_temp_expansion_data();
void                        free_temp_expansion_data(temp_expansion_data_t*);
void                        load_expansion_data(string, game_data_t*, temp_expansion_data_t*);
void                        determine_expansion_data_scope(string, temp_expansion_data_t*, internal_expansion_data_t*);
void                        parse_expansion_types(string, temp_expansion_data_t*, internal_expansion_data_t*);
void                        parse_expansion_tiles(string, temp_expansion_data_t*, internal_expansion_data_t*);
void                        parse_expansion_dice(string, temp_expansion_data_t*, internal_expansion_data_t*);
string                      parse_identifier(string, string);

void load_expansion_data(string expansion_name, game_data_t* game_data, temp_expansion_data_t* ted) {
    FILE*                       fptr;
    char                        line[MAX_LINE_LENGTH];
    internal_expansion_data_t*  internal;
    string                      path;

    path = join_path(2, game_data->data_path, expansion_name);

    if ((fptr = fopen(path, "r")) == NULL) {
        printf("Fatal error: Could not find expansion %s with file path %s.", expansion_name, path);
        exit(1);
    }
    free(path);

    internal = malloc(sizeof(internal_expansion_data_t));
    internal->expansion_name = copy_str(expansion_name);
    internal->identifier2index = init_robin_hash(10, 1000);

    ted->mode = OUTER_SCOPE;
    ted->type_scope = false;
    ted->tile_scope = false;
    ted->dice_scope = false;

    if (fgets(line, sizeof line, fptr) == NULL) {
        printf("Fatal error: Unexpected EOF.\n");
        exit(1);
    }

    internal->identifier = copy_str(parse_identifier(line, expansion_name));
    determine_expansion_data_scope(strip_to(line, '\0') + 1, ted, internal);

    DEBUG_PRINT(INFO, "Loading %s with identifier %s.\n", internal->expansion_name, internal->identifier);

    while (fgets(line, sizeof line, fptr) != NULL) {
        switch (ted->mode) {
            case OUTER_SCOPE:
                determine_expansion_data_scope(line, ted, internal);
                break;
            case TYPE_SCOPE:
                parse_expansion_types(line, ted, internal);
                break;
            case TILE_SCOPE:
                parse_expansion_tiles(line, ted, internal);
                break;
            case DICE_SCOPE:
                parse_expansion_dice(line, ted, internal);
                break;
            default:
                printf("Fatal error: Unknown read mode when parsing expansion data.\n");
                exit(1);
        }
    }
    fclose(fptr);

    free(internal->identifier);
    free(internal->expansion_name);
    free_robin_hash(internal->identifier2index);
    free(internal);
}

string parse_identifier(string line, string expansion_name) {
    string identifier, temp;

    identifier = strip_to(line, '<');

    if (*identifier == '\0') {
        return copy_str(expansion_name);
    }
    
    temp = strip_to(identifier, '>') - 1;
    *temp = '\0';

    if (identifier == temp) {
        return copy_str(expansion_name);
    }

    return identifier;
}

void determine_expansion_data_scope(string line, temp_expansion_data_t* ted, internal_expansion_data_t* internal) {
    string temp = strip_while(line, ' ');

    if (strstart("TYPES", temp)) {
        if (ted->type_scope) {
            printf("Fatal error: Type scope encountered more than once in file \"%s\".\n", internal->expansion_name);
            exit(1);
        }

        ted->type_scope = true;
        ted->mode = TYPE_SCOPE;
    } else if (strstart("TILES", temp)) {
        if (ted->tile_scope) {
            printf("Fatal error: Tile scope encountered more than once in file \"%s\".\n", internal->expansion_name);
            exit(1);
        }
        
        ted->tile_scope = true;
        ted->mode = TILE_SCOPE;
    } else if (strstart("DICE", temp)) {
        if (ted->dice_scope) {
            printf("Fatal error: Dice scope encountered more than once in file \"%s\".\n", internal->expansion_name);
            exit(1);
        }

        ted->dice_scope = true;
        ted->mode = DICE_SCOPE;
    } else if (!strstart("#", temp) && !strstart("\n", temp)){
        printf("Fatal error: Unknown read mode, got \"%s\".\n", temp);
        exit(1);
    }
}

void parse_expansion_types(string line, temp_expansion_data_t* ted, internal_expansion_data_t* internal) {
    int i;
    char c;
    string type;

    line = strip_while(line, ' ');
    if (line[0] == '}') {
        ted->mode = OUTER_SCOPE;
        return;
    }

    type = line;
    for (i = 0, c = line[i++]; line[i] != '\0'; c = line[i++]) {
        if (c == ' ' || c == '\n' || c == '\0' || c == '#') {
            line[i-1] = '\0';
            break;
        }
    }

    if (*type == '\0') return;

    append(ted->types, copy_str(type));
}

void parse_expansion_tiles(string line, temp_expansion_data_t* ted, internal_expansion_data_t* internal) {
    int curr_connection, station;
    bool final_connection;
    string temp;
    temp_tile_t* tile;

    line = strip_while(line, ' ');
    if (line[0] == '}') {
        ted->mode = OUTER_SCOPE;
        return;
    }
    
    if (line[0] == '#' || line[0] == '\n' || line[0] == '\0') return;

    tile = malloc(sizeof(temp_tile_t));
    tile->type = NULL;
    for (int j = 0; j < 4; j++) {
        tile->connections[j] = NULL;
    }

    tile->identifier = copy_str(parse_identifier(line, ""));
   
    if (tile->identifier[0] == '\0') {
        printf("Fatal error: Tile needs to start with an identifier.\n");
        exit(1);
    }

    line = strip_while(strip_to(line, '\0') + 1, ' ');
    temp = strip_to(line, ':');

    if (*temp == '\0') {
        printf("Fatal error: Tried to parse tile id of tile with identifier %s in expansion %s, no \":\" encountered.\n", tile->identifier, internal->expansion_name);
        exit(1);
    }

    *(temp - 1) = '\0';
    tile->id = copy_str(line);
    line = strip_to(temp, '(');
    curr_connection = 0;
    final_connection = false;

    while (true) {
        temp = line;
        while (*temp != ',' && *temp != ' ' && *temp != ')') temp++;
        if (*strip_while(temp, ' ') == ')') final_connection = true;
        *temp = '\0';

        tile->connections[curr_connection] = copy_str(line); 

        if (final_connection) break;
        line = strip_while(temp + 1, ' ');
        curr_connection += 1;

        if (curr_connection >= 4) {
            printf("Fatal error: Encountered too many connections in tile with identifier %s in expansion %s.\n", tile->identifier, internal->expansion_name);
            exit(1);
        }
    }

    if (curr_connection < 3) {
        printf("Fatal error: Encounterd too few connections (%d) in tile with identifier %s in expansion %s.\n", curr_connection + 1, tile->identifier, internal->expansion_name);
        exit(1);
    }

    line = strip_to(temp + 1, ',');
    line = strip_while(line, ' ');
    
    for (station = 0; station < sizeof tile->station; station++) {
        if (*line == '_') {
            tile->station[station] = false;
        } else if (*line == 'S') {
            tile->station[station] = true;
        } else {
            printf("Fatal error: Unknown station type \"%c\".\n", *line);
            exit(1);
        }

        line = strip_while(line + 1, ' ');
    }

    temp = str_concat("_", 2, internal->identifier, tile->identifier);
    add_key_u16(internal->identifier2index, hash_string(tile->identifier), ted->tiles->size);
    add_key_u16(ted->identifier2index, hash_string(temp), ted->tiles->size);
    free(temp);

    append(ted->tiles, tile);

    printf("<%s> %s: (", tile->identifier, tile->id);
    for (int i = 0; i < 4; i++) {
        printf("%s", tile->connections[i]);
        if (i != 3) printf(", ");
    }
    printf(") %d %d\n", tile->station[0], tile->station[1]);
}

void parse_expansion_dice(string line, temp_expansion_data_t* ted, internal_expansion_data_t* interal) {
    line = strip_while(line, ' ');
    if (line[0] == '}') {
        ted->mode = OUTER_SCOPE;
        return;
    }
}

temp_expansion_data_t* init_temp_expansion_data() {
    temp_expansion_data_t* ted;

    ted = malloc(sizeof(temp_expansion_data_t));
    ted->tiles = init_list();
    ted->types = init_list();
    ted->dice = init_list();
    ted->identifier2index = init_robin_hash(10, 1000);
    return ted;
}

void free_temp_expansion_data(temp_expansion_data_t* ted) {
    if (ted == NULL) return;

    free_list(ted->types, free);
    free_list(ted->tiles, free_temp_tile);
    free_list(ted->dice, free_temp_dice);
    free_robin_hash(ted->identifier2index);
    free(ted);
}

#endif
