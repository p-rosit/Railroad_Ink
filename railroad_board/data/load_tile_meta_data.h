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
                break;
            case TILES_SCOPE:
                break;
            case CONNECTIONS_SCOPE:
                break;
            case TRAVERSABLE_SCOPE:
                break;
            case NON_CONNECTIONS_SCOPE:
                break;
            case VALID_CONNECTIONS_SCOPE:
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
