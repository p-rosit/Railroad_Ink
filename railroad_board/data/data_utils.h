#ifndef DATA_UTILS_H
#define DATA_UTILS_H

#include <stdio.h>
#include <stdint.h>

#include "../utils/utils.h"
#include "../utils/debug_utils.h"

typedef struct settings     settings_t;
typedef struct expansions   expansions_t;

// Game data struct
typedef struct game_data    game_data_t;

struct game_data {
    string_t data_path;
    settings_t* settings;
    expansions_t* expansions;
};

struct settings {
    uint8_t max_combinations;
};

struct expansions {
    string_t* expansions;
    string_t* expansion_identifiers;
};

#endif
