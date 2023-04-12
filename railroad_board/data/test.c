#define DEBUG_LEVEL (1)

#include "data_utils.h"
#include "load_game_data.h"
#include "../utils/debug_utils.h"

int main() {
    game_data_t* game_data;

    game_data = load_game_data("./tiles");

    free(game_data);
    return 0;
}
