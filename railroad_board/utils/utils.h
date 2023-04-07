#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>

typedef struct array array_t;
typedef int    bool;
typedef void   (*free_func_t)(void*);

struct array {
    size_t size;
    void** data;
};

char*          copy_string(char*);
array_t*       init_array();
void           free_array(array_t*);

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

array_t* init_array() {
    array_t* arr = malloc(sizeof(array_t));
    arr->size = 0;
    arr->data = NULL;
}

void free_array(array_t* arr) {
    free(arr);
}

#endif
