#ifdef DEBUG
#ifndef DEBUG_UTILS_H
#define DEBUG_UTILS_H

#include <stdlib.h>
#include "utils.h"

void print_dictionary(dictionary_t* dict) {
    printf("dict (size: %lu, max_size: %lu): ", dict->size, dict->max_size);
    for (int i = 0; i < dict->key_amount; ++i) {
        printf("%lu ", dict->inds[i] % dict->size);
    }
    printf("\n");
    for (int i = 0; i < dict->key_amount; ++i) {
        printf("%d ", dict->vals[dict->inds[i] % dict->size]);
    }
    printf("\n");
}

#endif
#endif
