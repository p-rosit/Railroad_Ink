#include <stdlib.h>
#include <stdio.h>

#include "../railroad_types.h"
#include "../board/railroad_board.h"


tile_t make_tile() {
    size_t i;
    tile_t tile;
    
    tile.expansion_index = 1;
    tile.local_index = 2;
    tile.orientation = 3;
    tile.center = 4;
    for (i = 0; i < MAX_EXPANSIONS; i++) tile.data[i] = i + 5;
    for (i = 0; i < 4; i++) tile.connections[i] = i + 8;
    for (i = 0; i < 8; i++) tile.networks[i] = i + 12;

    return tile;
}


board_coord_t mc(board_size_t i, board_size_t j) {
    board_coord_t coord;
    coord.row = i;
    coord.col = j;
    return coord;
}


int main() {
    tile_t tile;
    board_t board;
    board_load_info_t info;

    info.height = 5;
    info.width = 9;
    for (size_t i = 0; i < MAX_EXPANSIONS; i++) {
        info.expansions[i] = NO_EXPANSION;
    }
    info.expansions[0] = e_STANDARD;

    board = make_board(info);
    tile = make_tile();

    add_tile_to_board(board, tile, mc(0, 2));
    add_tile_to_board(board, tile, mc(1, 2));
    add_tile_to_board(board, tile, mc(2, 2));

    printf("0\n"); print_board(board, 0);
    printf("1\n"); print_board(board, 1);
    printf("2\n"); print_board(board, 2);
    printf("3\n"); print_board(board, 3);

    printf("4\n"); print_board(board, 4);
    printf("5\n"); print_board(board, 5);
    printf("6\n"); print_board(board, 6);

    printf("7\n"); print_board(board, 7);
    printf("8\n"); print_board(board, 8);
    printf("9\n"); print_board(board, 9);
    printf("10\n"); print_board(board, 10);

    printf("11\n"); print_board(board, 11);
    printf("12\n"); print_board(board, 12);
    printf("13\n"); print_board(board, 13);
    printf("14\n"); print_board(board, 14);
    printf("15\n"); print_board(board, 15);
    printf("16\n"); print_board(board, 16);
    printf("17\n"); print_board(board, 17);
    printf("18\n"); print_board(board, 18);

    free_board(board);
    return 0;
}
