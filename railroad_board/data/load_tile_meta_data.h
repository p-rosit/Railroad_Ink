#ifndef LOAD_TILE_META_DATA_H
#define LOAD_TILE_META_DATA_H

#define TILES_SCOPE                 (1)
#define CONNECTIONS_SCOPE           (2)
#define TRAVERSABLE_SCOPE           (3)
#define NON_CONNECTIONS_SCOPE       (4)
#define VALID_CONNECTIONS_SCOPE     (5)

#include "../utils/utils.h"
#include "data_utils.h"

temp_tile_data_t*   load_tile_meta_data(game_data_t*);
void                determine_tile_meta_data_scope(string_t, temp_tile_data_t*);
void                parse_tiles(string_t, temp_tile_data_t*);
void                parse_connections(string_t, temp_tile_data_t*);
void                parse_traversable(string_t, temp_tile_data_t*);
void                parse_non_connections(string_t, temp_tile_data_t*);
void                parse_valid_connections(string_t, temp_tile_data_t*);

temp_tile_data_t* load_tile_meta_data(game_data_t* game_data) {
    FILE*               fptr;
    char                line[MAX_LINE_LENGTH];
    temp_tile_data_t*   ttd;
    string_t            path;

    path = join_path(3, game_data->data_path, "meta_data", "tile_info");

    if ((fptr = fopen(path, "r")) == NULL) {
        printf("Fatal error: Could not find tile meta data.");
        exit(1);
    }
    free(path);

    ttd = (temp_tile_data_t*) malloc(sizeof(temp_tile_data_t));
    ttd->mode = OUTER_SCOPE;
    ttd->tiles_scope = false;
    ttd->connections_scope = false;
    ttd->traversable_scope = false;
    ttd->non_connections_scope = false;
    ttd->valid_connections_scope = false;

    while (fgets(line, sizeof line, fptr) != NULL) {
        switch (ttd->mode) {
            case OUTER_SCOPE:
                printf("%s", line);
                determine_tile_meta_data_scope(line, ttd);
                break;
            case TILES_SCOPE:
                parse_tiles(line, ttd);
                break;
            case CONNECTIONS_SCOPE:
                parse_connections(line, ttd);
                break;
            case TRAVERSABLE_SCOPE:
                parse_traversable(line, ttd);
                break;
            case NON_CONNECTIONS_SCOPE:
                parse_non_connections(line, ttd);
                break;
            case VALID_CONNECTIONS_SCOPE:
                parse_valid_connections(line, ttd);
                break;
            default:
                printf("Fatal error: Unkown read mode when parsing tile meta data.\n");
                exit(1);
        }
    }
    fclose(fptr);

    game_data->expansions = NULL;
    return ttd;
}

void determine_tile_meta_data_scope(string_t line, temp_tile_data_t* ttd) {
    string_t string = strip_while(line, ' ');

    if (strstart("TILES", string)) {
        if (ttd->tiles_scope) {
            printf("Fatal error: Tiles scope encountered more than once.\n");
            exit(1);
        }
        ttd->mode = TILES_SCOPE;
        ttd->tiles_scope = true;
    } else if (strstart("CONNECTIONS", string)) {
        if (ttd->connections_scope) {
            printf("Fatal error: Connections scope encountered more than once.\n");
            exit(1);
        }
        ttd->mode = CONNECTIONS_SCOPE;
        ttd->connections_scope = true;
    } else if (strstart("TRAVERSABLE", string)) {
        if (ttd->traversable_scope) {
            printf("Fatal error: Traversable scope ecountered more than once.\n");
            exit(1);
        }
        ttd->mode = TRAVERSABLE_SCOPE;
        ttd->traversable_scope = true;
    } else if (strstart("NON_CONNECTIONS", string)) {
        if (ttd->non_connections_scope) {
            printf("Fatal error: Non Connections scope encountered more than once.\n");
            exit(1);
        }
        ttd->mode = NON_CONNECTIONS_SCOPE;
        ttd->non_connections_scope = true;
    } else if (strstart("VALID_CONNECTIONS", string)) {
        if (ttd->valid_connections_scope) {
            printf("Fatal error: Valid connections scope encountered more than once.\n");
            exit(1);
        }
        ttd->mode = VALID_CONNECTIONS_SCOPE;
        ttd->valid_connections_scope = true;
    } else if (!strstart("#", string) && !strstart("\n", string)) {
        printf("Fatal error: Unknown read mode found, got \"%s\"", line);
        exit(1);
    }
}

void parse_tiles(string_t line, temp_tile_data_t* ttd) {
    //printf("%s", line);
    line = strip_while(line, ' ');
    if (line[0] == '}') {
        ttd->mode = OUTER_SCOPE;
        return;
    }
}

void parse_connections(string_t line, temp_tile_data_t* ttd) {
    line = strip_while(line, ' ');
    if (line[0] == '}') {
        ttd->mode = OUTER_SCOPE;
        return;
    }
}

void parse_traversable(string_t line, temp_tile_data_t* ttd) {
    line = strip_while(line, ' ');
    if (line[0] == '}') {
        ttd->mode = OUTER_SCOPE;
        return;
    }
}

void parse_non_connections(string_t line, temp_tile_data_t* ttd) {
    line = strip_while(line, ' ');
    if (line[0] == '}') {
        ttd->mode = OUTER_SCOPE;
        return;
    }
}

void parse_valid_connections(string_t line, temp_tile_data_t* ttd) {
    //printf("%s", line);
    line = strip_while(line, ' ');
    if (line[0] == '}') {
        ttd->mode = OUTER_SCOPE;
        return;
    }
}

void free_tile_meta_data(game_data_t* game_data) {
    expansions_t* expansions = game_data->expansions;

    if (expansions != NULL) {
    
    }

    free(expansions);
}

void free_temp_tile_data(temp_tile_data_t* ttd) {
    free(ttd);
}

#endif
