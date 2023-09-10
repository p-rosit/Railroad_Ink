#include <stdlib.h>
#include <stdio.h>

#include "../railroad_types.h"
#include "../board/railroad_board.h"


Tile_t make_tile() {
    size_t i;
    Tile_t tile;
    
    tile.expansion_index = 1;
    tile.local_index = 2;
    tile.orientation = 3;
    tile.center = 4;
    for (i = 0; i < MAX_EXPANSIONS; i++) tile.data[i] = i + 5;
    for (i = 0; i < 4; i++) tile.connections[i] = i + 8;
    for (i = 0; i < 8; i++) tile.networks[i] = i + 12;

    return tile;
}


int main() {
    board_size_t height, width;
    Tile_t tile;
    Board_t board;

    height = 5;
    width = 9;

    board = make_board(height, width, 0);
    tile = make_tile();

    add_tile_to_board(board, tile, 0, 2);
    add_tile_to_board(board, tile, 1, 2);
    add_tile_to_board(board, tile, 2, 2);

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
