#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "board.h"

int main() {
    size_t amount;
    uint8_t* exps;
    expansion_list_t* exp_list;

    amount = 2;
    exps = malloc(amount * sizeof(uint8_t));
    exps[0] = STANDARD;
    exps[1] = LONGEST;

    exp_list = init_expansion_list(amount, exps);
    free(exps);

    printf("%lu\n", exp_list->amount);

    free_expansion_list(exp_list);
    return 0;
}

