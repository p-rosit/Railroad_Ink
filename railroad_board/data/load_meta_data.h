#ifndef LOAD_EXPANSIONS_H
#define LOAD_EXPANSIONS_H

#define SETTING_SCOPE       (1)
#define EXPANSION_SCOPE     (2)

#include <stdint.h>

#include "../utils/utils.h"
#include "../utils/debug_utils.h"
#include "../utils/linked_list.h"
#include "data_utils.h"

void    load_meta_data(game_data_t*);
int     determine_scope(string_t, bool_t*, bool_t*);
int     parse_settings(string_t, settings_t*);
int     parse_expansions(string_t, settings_t*, linked_list_t*);

void load_meta_data(game_data_t* game_data) {
    FILE*           fptr;
    int             mode;
    bool_t          setting_scope, expansion_scope;
    char            line[MAX_LINE_LENGTH];
    string_t        path;
    linked_list_t*  list;
    settings_t*     settings;

    path = join_path(3, game_data->data_path, "meta_data", "expansions");

    if ((fptr = fopen(path, "r")) == NULL) {
        printf("Fatal error: could not find expansion data.\n");
        exit(1);
    }
    free(path);

    list = init_list();
    settings = (settings_t*) malloc(sizeof(settings_t));

    mode = OUTER_SCOPE;
    setting_scope = false;
    expansion_scope = false;
    while (fgets(line, sizeof line, fptr) != NULL) {
        switch (mode) {
            case OUTER_SCOPE:
                mode = determine_scope(line, &setting_scope, &expansion_scope);
                break;
            case SETTING_SCOPE:
                mode = parse_settings(line, settings);
                break;
            case EXPANSION_SCOPE:
                mode = parse_expansions(line, settings, list);
                break;
            default:
                printf("Fatal error: Unkown read mode when parsing expansion data.\n");
                exit(1);
        }
    }
    fclose(fptr);

    if (mode != OUTER_SCOPE) {
        printf("Fatal error: Unpexpected EOF when parsing expansion data. Unfinished ");
        switch (mode) {
            case SETTING_SCOPE:
                printf("setting ");
                break;
            case EXPANSION_SCOPE:
                printf("expansion ");
                break;
            default:
                printf("unknown ");
        }
        printf("block.\n");
        exit(1);
    }

    list_element_t* elm = list->frst;
    for (mode = 0; mode < list->size; mode++) {
        printf("(%s) ", (string_t) elm->data);
        elm = elm->next;
    }
    printf("\n");

    free_list(list, free);
    game_data->settings = settings;
}

int determine_scope(string_t line, bool_t* setting_scope, bool_t* expansion_scope) {
    string_t string = strip_while(line, ' ');

    printf("%d: %s", OUTER_SCOPE, line);
    if (strstart("SETTINGS", string)) {
        return SETTING_SCOPE;
    } else if (strstart("EXPANSIONS", string)) {
        return EXPANSION_SCOPE;
    } else if (!strstart("#", string) && !strstart("\n", string)) {
        printf("Fatal error: Unknown read mode found, got \"%s\".\n", line);
        exit(1);
    }

    return OUTER_SCOPE;
}

int parse_settings(string_t line, settings_t* settings) {
    string_t pre_line, post_line;

    pre_line = strip_while(line, ' ');
    if (pre_line[0] == '}') {
        return OUTER_SCOPE;
    }
    post_line = strip_to(pre_line, ':');

    if (strstart("max_combinations", pre_line)) {
        settings->max_combinations = parse_uint(strip_while(post_line, ' '));
    } else if (pre_line[0] != '\n' && pre_line[0] != '#') {
        *(post_line - 1) = '\0';
        printf("Parse error: Skipping field \"%s\", not a known setting.\n", pre_line);
    }

    return SETTING_SCOPE;
}

int parse_expansions(string_t line, settings_t* settings, linked_list_t* list) {
    size_t size;
    char c;
    string_t expansion;

    line = strip_while(line, ' ');
    if (line[0] == '}') {
        return OUTER_SCOPE;
    }

    size = 0;
    line = strip_while(line, ' ');

    if (line[0] == '#' || line[0] == '\n' || line[0] == '\0') {
        return EXPANSION_SCOPE;
    }

    c = line[size++];
    while (c != ' ' && c != '#' && c != '\n' && c != '\0') {
        c = line[size++];
    }

    expansion = (string_t) malloc(size * sizeof(char));
    for (int i = 0; i < size; i++) {
        expansion[i] = line[i];
    }
    expansion[size - 1] = '\0';
    append(list, expansion);

    return EXPANSION_SCOPE;
}

void free_meta_data(game_data_t* game_data) {
    settings_t* settings = game_data->settings;

    if (settings == NULL) return;

    free(settings);
}

#endif

