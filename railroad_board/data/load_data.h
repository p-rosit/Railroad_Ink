#ifndef LOAD_DATA_H
#define LOAD_DATA_H

#include <stdio.h>

#include "../utils.h"
#include "data_utils.h"
#include "../mechanics/expansion_utils.h"

char*    find_file_name(enum expansion_id);
array_t* load_tiles(enum expansion_id*);

array_t* load_info(char* file) {
    FILE* fptr;

    if ((fptr = fopen(file, "r")) == NULL) {

    }

    fclose(fptr);
}

array_t* load_tiles(enum expansion_id* expansions) {
    
}

char* find_file_name(enum expansion_id) {
    return "./tiles/standard";
}

#endif

