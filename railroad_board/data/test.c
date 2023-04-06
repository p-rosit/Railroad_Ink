#define DEBUG

#include "data_utils.h"
#include "load_data.h"
#include "../debug_utils.h"

int main() {
    game_data_t* data;
    tile_data_t* tiles;

    data = malloc(sizeof(game_data_t));
    tiles = malloc(sizeof(tile_data_t));

    data->tiles = tiles;

    FILE* fptr;

    if ((fptr = fopen("./tiles/standard", "r")) == NULL) {
        printf("Could not open file.\n");
        exit(1);
    }

    dictionary_t* dict = init_dictionary(100);

    print_dictionary(dict);

    add_num(dict, "C", 0);

    print_dictionary(dict);

    add_num(dict, "CC", 1);

    print_dictionary(dict);

    add_num(dict, "T", 2);

    print_dictionary(dict);

    add_num(dict, "O", 3);

    print_dictionary(dict);

    add_num(dict, "i", 4);

    print_dictionary(dict);

    add_num(dict, "I", 5);

    print_dictionary(dict);

    fclose(fptr);
    free(data);
    free(tiles);
    free_dictionary(dict);

    return 0;
}
