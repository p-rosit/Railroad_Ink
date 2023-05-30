#ifndef LOAD_EXPANSIONS_H
#define LOAD_EXPANSIONS_H

#define SETTING_SCOPE       (1)
#define EXPANSION_SCOPE     (2)

#include <stdint.h>

#include "../utils/utils.c"
#include "../utils/debug_utils.c"
#include "../utils/linked_list.c"
#include "data_utils.c"

temp_meta_data_t*   load_meta_data(game_data_t*);
void                determine_expansion_scope(string, temp_meta_data_t*);
void                parse_settings(string, settings_t*, temp_meta_data_t*);
void                parse_expansions(string, settings_t*, temp_meta_data_t*);

temp_meta_data_t* load_meta_data(game_data_t* game_data) {
    FILE*               fptr;
    char                line[MAX_LINE_LENGTH];
    string              path;
    temp_meta_data_t*   tmd;
    settings_t*         settings;

    path = join_path(3, game_data->settings->data_path, "meta_data", "expansions");

    if ((fptr = fopen(path, "r")) == NULL) {
        printf("Fatal error: could not find expansion data.\n");
        exit(1);
    }
    free(path);

    tmd = malloc(sizeof(temp_meta_data_t));
    tmd->expansion_files = init_list();
    settings = game_data->settings;

    tmd->mode = OUTER_SCOPE;
    tmd->setting_scope = false;
    tmd->expansion_scope = false;
    while (fgets(line, sizeof line, fptr) != NULL) {
        switch (tmd->mode) {
            case OUTER_SCOPE:
                determine_expansion_scope(line, tmd);
                break;
            case SETTING_SCOPE:
                parse_settings(line, settings, tmd);
                break;
            case EXPANSION_SCOPE:
                parse_expansions(line, settings, tmd);
                break;
            default:
                printf("Fatal error: Unkown read mode when parsing expansion data.\n");
                exit(1);
        }
    }
    fclose(fptr);

    if (tmd->mode != OUTER_SCOPE) {
        printf("Fatal error: Unpexpected EOF when parsing expansion data. Unfinished ");
        switch (tmd->mode) {
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

    DEBUG_PRINT(INFO, "Found %lu expansions to load:\n", tmd->expansion_files->size);

    list_element_t* elm = tmd->expansion_files->frst;
    for (int i = 0; i < tmd->expansion_files->size; i++) {
        DEBUG_PRINT(INFO, "%s", (string) elm->data);
        if (i != tmd->expansion_files->size - 1) DEBUG_PRINT(INFO, ", ");
        elm = elm->next;
    }
    DEBUG_PRINT(INFO, "\n");

    game_data->settings = settings;
    return tmd;
}

void determine_expansion_scope(string line, temp_meta_data_t* tmd) {
    string str = strip_while(line, ' ');

    if (strstart("SETTINGS", str)) {
        if (tmd->setting_scope) {
            printf("Fatal error: Settings scope encountered more than once.\n");
            exit(1);
        }
        tmd->setting_scope = true;
        tmd->mode = SETTING_SCOPE;
    } else if (strstart("EXPANSIONS", str)) {
        if (tmd->expansion_scope) {
            printf("Fatal error: Expansions scope encountered more than once.\n");
            exit(1);
        }
        tmd->expansion_scope = true;
        tmd->mode = EXPANSION_SCOPE;
    } else if (!strstart("#", str) && !strstart("\n", str)) {
        printf("Fatal error: Unknown read mode found, got \"%s\".\n", line);
        exit(1);
    }
}

void parse_settings(string line, settings_t* settings, temp_meta_data_t* tmd) {
    string pre_line, post_line;

    pre_line = strip_while(line, ' ');
    if (pre_line[0] == '}') {
        tmd->mode = OUTER_SCOPE;
        return;
    }
    post_line = strip_to(pre_line, ':');

    if (strstart("max_combinations", pre_line)) {
        settings->max_combinations = parse_uint(strip_while(post_line, ' '));
    } else if (strstart("empty_type", pre_line)) {
        post_line = strip_while(post_line, ' ');
        pre_line = post_line;
        while (*pre_line != ' ' && *pre_line != '#' && *pre_line != '\n' && *pre_line != '\0') pre_line++;
        *pre_line = '\0';
        tmd->empty_type = copy_str(post_line);
    } else if (pre_line[0] != '\n' && pre_line[0] != '#') {
        *(post_line - 1) = '\0';
    }
}

void parse_expansions(string line, settings_t* settings, temp_meta_data_t* tmd) {
    size_t size;
    char c;
    string expansion;

    line = strip_while(line, ' ');
    if (line[0] == '}') {
        tmd->mode = OUTER_SCOPE;
        return;
    }

    size = 0;
    line = strip_while(line, ' ');

    if (line[0] == '#' || line[0] == '\n' || line[0] == '\0') {
        return;
    }

    c = line[size++];
    while (c != ' ' && c != '#' && c != '\n' && c != '\0') {
        c = line[size++];
    }

    expansion = malloc(size * sizeof(char));
    for (int i = 0; i < size; i++) {
        expansion[i] = line[i];
    }
    expansion[size - 1] = '\0';
    append(tmd->expansion_files, expansion);
}

void free_temp_meta_data(temp_meta_data_t* tmd) {
    if (tmd == NULL) return;

    free(tmd->empty_type);
    free_list(tmd->expansion_files, free);
    free(tmd);
}

void free_settings(settings_t* settings) {
    if (settings == NULL) return;

    free(settings->data_path);
    free(settings);
}

#endif

