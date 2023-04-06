#ifndef LOAD_DATA_H
#define LOAD_DATA_H

#include <stdlib.h>
#include <stdio.h>

#include "../utils.h"
#include "data_utils.h"
#include "../mechanics/expansion_utils.h"

char* find_file_name(expansion_id_t);
void  load_info(char* file);
void  load_tiles(expansion_id_t*);

void load_info(char* file) {
    FILE* fptr;

    if ((fptr = fopen(file, "r")) == NULL) {
        printf("File or path does not exist: %s", file);
        exit(1);
    }

    fclose(fptr);
}

void load_tiles(expansion_id_t* expansions) {
    
}

char* find_file_name(expansion_id_t expansion) {
    return "./tiles/standard";
}

#endif

