#define DEBUG_LEVEL (1)

#include "data_utils.c"
#include "load_game_data.c"

int main() {
    game_data_t* game_data;

    game_data = load_game_data("./tiles");
 
    free_game_data(game_data);
    return 0;
}
