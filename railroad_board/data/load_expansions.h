#ifndef EXPANSION_INFO_H
#define EXPANSION_INFO_H

#include <stdlib.h>
#include <stdio.h>

#include "../utils/debug_utils.h"
#include "../utils/hash_map.h"

#define MAX_EXP_HASH (1000)
#define MAX_EXPS     (50)

char* tile_path = "./tiles";

void load_info(uint8_t*, char***, hash_map_t** map);

void load_info(uint8_t* amount, char*** expansion_name, hash_map_t** map) {
    FILE* fptr;
    size_t i, j;
    char line[100], name[30], *file_name, c;

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

    DEBUG_PRINT(INFO, "Found %d expansions:\n%s (%d)", *amount, (*expansion_name)[0], get_num(*map, (*expansion_name)[0]));
    for (i = 1; i < *amount; i++) {
        DEBUG_PRINT(INFO, ", %s (%d)", (*expansion_name)[i], get_num(*map, (*expansion_name)[i]));
    }
    DEBUG_PRINT(INFO, "%s\n\n", " ");
}

#endif
