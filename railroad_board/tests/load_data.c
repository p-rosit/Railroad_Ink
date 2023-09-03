#include <stdio.h>
#include "../data/railroad_data.h"

int main() {
    size_t i;
    const GameData_t game_data = load_data(3, e_RIVER, e_METEOR, e_LAKE);

    for (i = 0; i < DEFAULT_EXPANSIONS + MAX_EXPANSIONS; i++) {
        printf("%lu: %lu\n", i, game_data.expansion_index[i]);
    }

    free_data(game_data);
    return 0;
}
