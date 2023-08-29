#include <stdlib.h>
#include <stdio.h>

#include "railroad_types.h"
#include "railroad_structs.h"
#include "railroad_data.c"


void remote_compatible(Board_t board, TileData_t tile_data, size_t i, size_t j) {
}

void write_local() {
}

void local_compatible(Board_t board, size_t i, size_t j) {
}

int main() {
    size_t depth;
    board_size_t height, width;
    height = 12;
    width = 12;
    depth = 4; /* 4 or 16 */

    board_data_t data[depth * (height + 2) * (width + 2)];

    Board_t board = (Board_t) {
        .height = height,
        .width = width,
        .board = data,
    };

    printf("%lu\n", depth * (height + 2) * (width + 2));

    return 0;
}
