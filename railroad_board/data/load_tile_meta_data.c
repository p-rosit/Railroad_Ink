#ifndef LOAD_TILE_META_DATA_H
#define LOAD_TILE_META_DATA_H

#include <stdlib.h>
#include <string.h>
#define TILES_SCOPE                 (1)
#define CONNECTIONS_SCOPE           (2)
#define TRAVERSABLE_SCOPE           (3)
#define NON_CONNECTIONS_SCOPE       (4)
#define VALID_CONNECTIONS_SCOPE     (5)

#include "../utils/utils.c"
#include "data_utils.c"

temp_tile_data_t*   load_tile_meta_data(game_data_t*);
void                determine_tile_meta_data_scope(string, temp_tile_data_t*);
void                parse_tiles(string, temp_tile_data_t*);
void                parse_connections(string, temp_tile_data_t*);
void                parse_traversable(string, temp_tile_data_t*);
void                parse_non_connections(string, temp_tile_data_t*);
void                parse_valid_connections(string, temp_tile_data_t*);

temp_tile_data_t* load_tile_meta_data(game_data_t* game_data) {
    FILE*               fptr;
    char                line[MAX_LINE_LENGTH];
    temp_tile_data_t*   ttd;
    string              path;

    path = join_path(3, game_data->data_path, "meta_data", "tile_info");

    if ((fptr = fopen(path, "r")) == NULL) {
        printf("Fatal error: Could not find tile meta data.");
        exit(1);
    }
    free(path);

    ttd = malloc(sizeof(temp_tile_data_t));
    ttd->mode                       = OUTER_SCOPE;
    ttd->tiles_scope                = false;
    ttd->connections_scope          = false;
    ttd->traversable_scope          = false;
    ttd->non_connections_scope      = false;
    ttd->valid_connections_scope    = false;

    ttd->tile_ids                   = init_list();
    ttd->tile_networks              = init_list();
    ttd->tile_connections           = init_list();
    ttd->traversable_connections    = init_list();
    ttd->non_connections            = init_list();
    ttd->valid_connections          = init_list();

    while (fgets(line, sizeof line, fptr) != NULL) {
        switch (ttd->mode) {
            case OUTER_SCOPE:
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

    list_element_t* elm = ttd->tile_ids->frst;
    printf("Ids: ");
    for (int i = 0; i < ttd->tile_ids->size; i++, elm = elm->next) {
        printf("%s ", (string) elm->data);
    }
    printf("\n");

    printf("Connections: ");
    elm = ttd->tile_connections->frst;
    for (int i = 0; i < ttd->tile_connections->size; i++, elm = elm->next) {
        printf("%s ", (string) elm->data);
    }
    printf("\n");

    printf("Traversable: ");
    elm = ttd->traversable_connections->frst;
    for (int i = 0; i < ttd->traversable_connections->size; i++, elm = elm->next) {
        printf("%s ", (string) elm->data);
    }
    printf("\n");

    printf("Non Connections: ");
    elm = ttd->non_connections->frst;
    for (int i = 0; i < ttd->non_connections->size; i++, elm = elm->next) {
        printf("%s ", (string) elm->data);
    }
    printf("\n");

    elm = ttd->tile_networks->frst;
    list_element_t* nelm;
    for (int i = 0; i < ttd->tile_networks->size; i++, elm = elm->next) {
        nelm = ((linked_list_t*) (elm->data))->frst;
        printf("%d: ", i);
        for (int j = 0; j < ((linked_list_t*) (elm->data))->size; j++, nelm = nelm->next) {
            for (int k = 0; k < 4; k++) {
                if (((bool*) nelm->data)[k]) {
                    printf("1");
                } else {
                    printf("0");
                }
            }
            printf(" ");
        }
        printf("\n");
    }

    game_data->expansions = NULL;
    return ttd;
}

void determine_tile_meta_data_scope(string line, temp_tile_data_t* ttd) {
    string string = strip_while(line, ' ');

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

void parse_tiles(string line, temp_tile_data_t* ttd) {
    string name, temp;
    linked_list_t *nets;
    bool* net;
    char c;

    line = strip_while(line, ' ');
    if (line[0] == '}') {
        ttd->mode = OUTER_SCOPE;
        return;
    }

    temp = strip_to(line, ':');
    *(temp - 1) = '\0';

    name = copy_str(line);
    append(ttd->tile_ids, name);
    line = strip_while(strip_to(temp, '('), ' ');

    nets = init_list();
    c = line[0];
    while (c != ')' && c != '\0') {
        net = calloc(4, sizeof(bool));

        c = line[0];
        while (c != ')' && c != '\0') {
            line = strip_while(line, ' ');
            temp = line;
            c = *temp;

            while (c != ',' && c != ')' && c != '\0') {
                c = *(temp++);
            }
            if (c == '\0') {
                printf("Fatal error: Unexpected end of line, network should end with \")\"\n");
                exit(1);
            }
            *(temp - 1) = '\0';
            
            if (streq(line, "North")) {
                net[0] = true;
            } else if (streq(line, "East")) {
                net[1] = true;
            } else if (streq(line, "South")) {
                net[2] = true;
            } else if (streq(line, "West")) {
                net[3] = true;
            } else {
                printf("Fatal error: Unknown direction in network encountered. Expected \"North\", \"East\", \"South\" or \"West\" but got \"%s\".\n", line);
                exit(1);
            }
            line = temp;
            if (c == ')') {
                append(nets, net);
                line = strip_to(line, '(');
                
                if (line[0] != '\0') {
                    c = '(';
                }
                break;
            }
        }
    }
    append(ttd->tile_networks, nets);
}

void parse_connections(string line, temp_tile_data_t* ttd) {
    string temp;

    line = strip_while(line, ' ');
    if (line[0] == '}') {
        ttd->mode = OUTER_SCOPE;
        return;
    }

    temp = separate_name(line);
    if (line[0] == '\0') {
        return;
    }

    append(ttd->tile_connections, copy_str(line));
}

void parse_traversable(string line, temp_tile_data_t* ttd) {
    string temp;

    line = strip_while(line, ' ');
    if (line[0] == '}') {
        ttd->mode = OUTER_SCOPE;
        return;
    }

    temp = separate_name(line);
    if (line[0] == '\0') {
        return;
    }

    append(ttd->traversable_connections, copy_str(line));
}

void parse_non_connections(string line, temp_tile_data_t* ttd) {
    string temp;

    line = strip_while(line, ' ');
    if (line[0] == '}') {
        ttd->mode = OUTER_SCOPE;
        return;
    }
    
    temp = separate_name(line);
    if (line[0] == '\0') {
        return;
    }

    append(ttd->non_connections, copy_str(line));
}

void parse_valid_connections(string line, temp_tile_data_t* ttd) {
    string temp;

    line = strip_while(line, ' ');
    if (line[0] == '}') {
        ttd->mode = OUTER_SCOPE;
        return;
    }

    if (*(line++) != '(') {
        return;
    }

    temp = separate_name(line);
    append(ttd->valid_connections, copy_str(line));

    printf("%s\n", line);
}

void free_tile_meta_data(game_data_t* game_data) {
    expansions_t* expansions = game_data->expansions;

    if (expansions != NULL) {
    
    }

    free(expansions);
}

void free_temp_tile_data(temp_tile_data_t* ttd) {
    list_element_t *elm, *nxt, *e, *n;

    free_list(ttd->tile_ids, free);
    free_list(ttd->tile_connections, free);
    free_list(ttd->traversable_connections, free);
    free_list(ttd->non_connections, free);
    free_list(ttd->valid_connections, free);
    elm = ttd->tile_networks->frst;
    for (elm = ttd->tile_networks->frst; elm != NULL; elm = nxt) {
        nxt = elm->next;
        free_list((linked_list_t*) elm->data, free);
        free(elm);
    }
    free(ttd->tile_networks);

    free(ttd);
}

#endif
