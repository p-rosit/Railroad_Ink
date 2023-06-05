#ifndef LOAD_EXPANSION_DATA_H
#define LOAD_EXPANSION_DATA_H

#include <stdio.h>
#include <stdlib.h>
#define TYPE_SCOPE (1)
#define TILE_SCOPE (2)
#define DICE_SCOPE (3)

#include "../utils/utils.c"
#include "../utils/debug_utils.c"
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
void                        parse_expansion_types(string, game_data_t*, temp_expansion_data_t*, internal_expansion_data_t*);
void                        parse_expansion_tiles(string, temp_expansion_data_t*, internal_expansion_data_t*);
void                        parse_expansion_dice(string, game_data_t*, temp_expansion_data_t*, internal_expansion_data_t*);
string                      parse_identifier(string, string);

void load_expansion_data(string expansion_name, game_data_t* game_data, temp_expansion_data_t* ted) {
    FILE*                       fptr;
    char                        line[MAX_LINE_LENGTH];
    internal_expansion_data_t*  internal;
    string                      path;
    linked_list_t*              type_list;

    path = join_path(2, game_data->settings->data_path, expansion_name);

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
    type_list = init_list();
    append(ted->types, type_list);

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
                parse_expansion_types(line, game_data, ted, internal);
                break;
            case TILE_SCOPE:
                parse_expansion_tiles(line, ted, internal);
                break;
            case DICE_SCOPE:
                parse_expansion_dice(line, game_data, ted, internal);
                break;
            default:
                printf("Fatal error: Unknown read mode when parsing expansion data.\n");
                exit(1);
        }
    }
    fclose(fptr);

    type_list = ted->types->last->data;
    if (type_list->size == 0) {
        free_list(remove_last(ted->types), NULL);
    }

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

void parse_expansion_types(string line, game_data_t* game_data, temp_expansion_data_t* ted, internal_expansion_data_t* internal) {
    int i;
    char c;
    linked_list_t* type_list;
    string type;

    line = strip_while(line, ' ');
    if (line[0] == '}') {
        ted->mode = OUTER_SCOPE;
        return;
    }

    if (line[0] == '#') return;

    type = line;
    for (i = 0, c = line[i]; line[i] != '\0'; c = line[++i]) {
        if (c == ' ' || c == '\n' || c == '\0' || c == '#') {
            line[i] = '\0';
            break;
        }
    }

    if (*type == '\0') return;

    if (key_exists(game_data->map->type, hash_string(type))) {
        printf("Fatal error: Tile type \"%s\" already exists.\n", type);
        exit(1);
    }

    type_list = ted->types->last->data;

    add_key_u16(game_data->map->type, hash_string(type), ted->total_types);
    append(type_list, copy_str(type));

    DEBUG_PRINT(INFO, "Type %s with number %d.\n", (string) type_list->last->data, get_val_u16(game_data->map->type, hash_string(type_list->last->data)));

    ted->total_types += 1;
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
    tile->identifier = copy_str(parse_identifier(line, ""));
   
    if (tile->identifier[0] == '\0') {
        printf("Fatal error: Tile needs to start with a non-empty identifier.\n");
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

    line = strip_while(strip_to(line, '\0') + 1, ' ');
    temp = strip_to(line, ',');
    *(temp - 1) = '\0';

    tile->type = copy_str(line);

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

    if (key_exists(internal->identifier2index, hash_string(tile->identifier))) {
        printf("Fatal error: Duplicate identifier found: \"%s\".\n", tile->identifier);
        exit(1);
    }

    if (key_exists(ted->identifier2index, hash_string(tile->identifier))) {
        DEBUG_PRINT(WARN, "Tile with internal identifier \"%s\" and external identifier \"%s\" has ambiguous name. The identifier \"%s\" appears in the external namespace.\n", tile->identifier, temp, tile->identifier);
    }

    add_key_u16(internal->identifier2index, hash_string(tile->identifier), ted->tiles->size);
    add_key_u16(ted->identifier2index, hash_string(temp), ted->tiles->size);
    free(temp);

    append(ted->tiles, tile);

    DEBUG_PRINT(INFO, "<%s> %2s: %s (", tile->identifier, tile->id, tile->type);
    for (int i = 0; i < 4; i++) {
        DEBUG_PRINT(INFO, "%s", tile->connections[i]);
        if (i != 3) DEBUG_PRINT(INFO, ", ");
    }
    DEBUG_PRINT(INFO, ") %d %d\n", tile->station[0], tile->station[1]);
}

void parse_expansion_dice(string line, game_data_t* game_data, temp_expansion_data_t* ted, internal_expansion_data_t* internal) {
    string temp, identifier;
    int curr_dice, final_dice;
    temp_dice_t* dice;

    line = strip_while(line, ' ');
    if (line[0] == '}') {
        ted->mode = OUTER_SCOPE;
        return;
    }

    if (line[0] == '#' || line[0] == '\n' || line[0] == '\0') return;

    dice = malloc(sizeof(temp_dice_t));
    dice->identifier = copy_str(parse_identifier(line, ""));
   
    if (dice->identifier[0] == '\0') {
        printf("Fatal error: Dice needs to start with a non-empty identifier.\n");
        exit(1);
    }

    line = strip_to(strip_to(line, '\0') + 1, '(');
    curr_dice = 0;
    final_dice = false;

    while (true) {
        temp = line;
        while (*temp != ',' && *temp != ' ' && *temp != ')') temp++;
        if (*strip_while(temp, ' ') == ')') final_dice = true;
        *temp = '\0';

        identifier = line;
        if (key_exists(internal->identifier2index, hash_string(identifier))) {
            identifier = str_concat("_", 2, internal->identifier, identifier);
        } else {
            identifier = copy_str(identifier);
        }
        dice->dice[curr_dice] = identifier;

        if (final_dice) break;
        line = strip_while(temp + 1, ' ');
        curr_dice += 1;

        if (curr_dice >= 6) {
            printf("Fatal error: Encountered too many dice in dice with identifier %s in expansion %s.\n", dice->identifier, internal->expansion_name);
            exit(1);
        }
    }

    if (curr_dice < 5) {
        printf("Fatal error: Encounterd too few connections (%d) in tile with identifier %s in expansion %s.\n", curr_dice + 1, dice->identifier, internal->expansion_name);
        exit(1);
    }

    add_key_u16(game_data->map->dice, hash_string(dice->identifier), ted->dice->size);
    append(ted->dice, dice);

    printf("<%s>: (", dice->identifier);
    for (int i = 0; i < 6; i++) {
        if (i > 0) printf(", ");
        printf("%s", dice->dice[i]);
    }
    printf(")\n");
}

temp_expansion_data_t* init_temp_expansion_data() {
    temp_expansion_data_t* ted;

    ted = malloc(sizeof(temp_expansion_data_t));
    ted->total_types = 0;
    ted->tiles = init_list();
    ted->types = init_list();
    ted->dice = init_list();
    ted->identifier2index = init_robin_hash(10, 1000);
    return ted;
}

void free_temp_expansion_data(temp_expansion_data_t* ted) {
    list_element_t *elm, *nxt;
    if (ted == NULL) return;

    for (elm = ted->types->frst; elm != NULL; elm = nxt) {
        nxt = elm->next;
        free_list(elm->data, free);
        free(elm);
    }
    free(ted->types);
    free_list(ted->tiles, free_temp_tile);
    free_list(ted->dice, free_temp_dice);
    free_robin_hash(ted->identifier2index);
    free(ted);
}

#endif
