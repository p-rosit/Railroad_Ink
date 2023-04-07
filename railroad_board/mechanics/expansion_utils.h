#ifndef EXPANSION_UTILS_H
#define EXPANSION_UTILS_H
#include <stdint.h>
#include "../utils/utils.h"

typedef enum expansion_id {
    STANDARD,
    NETWORK,
    LONGEST,
    CENTER,
    ERROR,
    SPECIAL,
    MOUNTAIN,
    BUILDING
} expansion_id_t;

typedef struct expansion expansion_t;
typedef struct expansion_list expansion_list_t;

typedef expansion_t* (*init_func_t)(expansion_list_t*);

struct expansion {
    expansion_list_t* exp_list;
    free_func_t free_func;
    uint16_t points;
};

struct expansion_list {
    size_t amount;
    uint8_t* id;
    expansion_t** expansion;
};

#endif
