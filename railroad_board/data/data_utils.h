#ifndef DATA_UTILS_H
#define DATA_UTILS_H
#include <stdint.h>
#include "../utils/utils.h"
#include "../utils/linked_list.h"

#include <stdio.h>

typedef struct game_data     game_data_t;
typedef struct tile_info     tile_info_t;
typedef struct tile_data     tile_data_t;
typedef struct single_tile   single_tile_t;
typedef struct type_data     type_data_t;
typedef struct tile_textures tile_textures_t;

struct game_data {
    tile_info_t* info;
    tile_data_t* tiles;
    type_data_t* types;
    tile_textures_t* tile_textures;
};

struct tile_info {
    uint8_t id_amount;
    uint8_t connection_amount;
    bool* networks;
    bool* traversable;
    bool* non_connection;
    bool* valid_connection;
};

struct single_tile {
    uint8_t id;
    uint8_t type;
    uint8_t connections[4];
    bool station;
};

struct tile_data {
    uint8_t expansion_amount;
    uint16_t* expansion_ind;
    uint8_t* ids;
    uint8_t* types;
    uint8_t* connections;
    bool* stations;
};

struct type_data {
    uint8_t max_combinations;
    uint16_t* combination_ind;
    uint8_t* index2types;
    uint8_t* types2index;
};

struct tile_textures {
    uint8_t* textures; // TODO
};

char*        concat_string(char*, char*);
bool         string_starts_with(char*, char*);
bool*        parse_internal_networks(char*); 
int          parse_single_network(char*, int, bool*);
 
char* concat_string(char* a, char* b) {
    size_t len;
    int i, j;
    char c, *res;

    len = 0; i = 0; j = 0;
    while(a[i++] != '\0') len++;
    while(b[j++] != '\0') len++;
    len += 2;

    res = malloc(len * sizeof(char));

    i = 0; j = 0;
    while ((c = a[i++]) != '\0') res[j++] = c;
    res[j++] = '/';
    i = 0;
    while ((c = b[i++]) != '\0') res[j++] = c;
    res[j++] = '\0';

    return res;
}

bool string_starts_with(char* sub, char* string) {
    int i;
    bool match;

    i = 0;
    match = true;
    while (sub[i] != '\0') {
        if (sub[i] != string[i]) {
            match = false;
            break;
        }
        i++;
    }

    return match;
}

bool* parse_internal_networks(char* line) {
    int i;
    char c;
    bool* networks;

    networks = calloc(8, sizeof(bool));

    i = 0;
    while (line[i++] != '(');
    i = parse_single_network(line, i, networks);
    while ((c = line[i++]) != '(') {
        if (c == '#' || c == '\0') return networks;
    }
    i = parse_single_network(line, i, networks+4);

    return networks;
}

int parse_single_network(char* line, int ind, bool* network) {
    int i, j;
    char word[6];

    while (line[ind] != ')') {
        i = 0;
        while (line[ind] != ' ' && line[ind] != ',' && line[ind] != ')') {
            word[i++] = line[ind];
            ind++;
        }
        word[i] = '\0';

        j = streq(word, "North") + 2 * streq(word, "East") + 3 * streq(word, "South") + 4 * streq(word, "West");
        if (j > 0) {
            network[j - 1] = true;
        } else {
            printf("Unreachable error: %s is an invalid direction.", word);
            exit(1);
        }

        if (line[ind] == ')') break;
        while (line[ind] == ',' || line[ind] == ' ') {
            ind++;
        }
    }

    return ind;
}

#endif
