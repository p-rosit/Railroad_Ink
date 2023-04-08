#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>
#include <string.h>

typedef struct array array_t;
typedef int    bool;
typedef void   (*free_func_t)(void*);

enum debug_levels {
    DEBUG = 0,
    INFO  = 1,
    WARN  = 2,
    ERROR = 3,
    FATAL = 4
};

struct array {
    size_t size;
    void** data;
};

char*          copy_string(char*);
array_t*       init_array();
void           free_array(array_t*);

const bool false = 0;
const bool true = 1;

char* copy_string(char* string) {
    size_t size;
    char c, *traverse, *copy;

    size = 0;
    while ((c = string[size++]) != '\0');

    copy = malloc(size * sizeof(char));
    for (int i = 0; i < size; ++i) {
        copy[i] = string[i];
    }

    return copy;
}

bool streq(char* a, char* b) {
    return strcmp(a, b) == 0;
}

array_t* init_array() {
    array_t* arr = malloc(sizeof(array_t));
    arr->size = 0;
    arr->data = NULL;
}

void free_array(array_t* arr) {
    free(arr);
}

#endif
