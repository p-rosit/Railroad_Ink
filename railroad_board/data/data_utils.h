#ifndef DATA_UTILS_H
#define DATA_UTILS_H

#define LOAD_EXPANSION_FUNCTION(name, amount, ...) \
\
size_t load_##name##_tiles(board_data_t* tile_data, size_t index) { \
    const board_data_t data[TILE_DATA_WIDTH * amount] = {__VA_ARGS__}; \
    for (size_t i = index; i < index + TILE_DATA_WIDTH * amount; i++) { \
        tile_data[i] = data[i - index]; \
    } \
    return index + TILE_DATA_WIDTH * amount; \
}

#define TRAVERSABLE_DATA \
{ \
    0,  /*  E */ \
    1,  /*  C */ \
    1,  /*  R */ \
    0,  /*  M */ \
    0,  /*  P */ \
    0,  /* Ri */ \
    0,  /* La */ \
    0,  /* Lv */ \
}

#define NON_CONNECTION_DATA \
{ \
    1,  /*  E */ \
    0,  /*  C */ \
    0,  /*  R */ \
    1,  /*  M */ \
    1,  /*  P */ \
    0,  /* Ri */ \
    0,  /* La */ \
    0,  /* Lv */ \
}

#define VALID_CONNECTIONS_DATA \
{ \
    /*          E   C   R   M   P   Ri  La  Lv  */ \
    /*  E */    1,  1,  1,  1,  1,  1,  1,  1, \
    /*  C */    1,  1,  0,  1,  1,  0,  0,  0, \
    /*  R */    1,  0,  1,  1,  1,  0,  0,  0, \
    /*  M */    1,  0,  0,  0,  0,  0,  0,  0, \
    /*  P */    0,  1,  1,  0,  0,  1,  0,  0, \
    /* Ri */    1,  0,  0,  1,  1,  1,  0,  0, \
    /* La */    1,  0,  0,  1,  1,  0,  1,  0, \
    /* Lv */    1,  0,  0,  1,  1,  0,  0,  1, \
}

#define CONNECTION_DATA \
{ \
    /*          First network       Second network */ \
    /*  E */    0,  0,  0,  0,      0,  0,  0,  0, \
    /* Ep */    0,  0,  0,  0,      0,  0,  0,  0, \
    /*  C */    1,  1,  0,  0,      0,  0,  0,  0, \
    /*  i */    1,  0,  0,  0,      0,  0,  0,  0, \
    /*  I */    1,  0,  1,  0,      0,  0,  0,  0, \
    /*  T */    1,  1,  1,  0,      0,  0,  0,  0, \
    /* dT */    1,  0,  1,  0,      0,  1,  0,  0, \
    /* CC */    1,  1,  0,  0,      0,  0,  1,  1, \
    /*  O */    1,  0,  1,  0,      0,  1,  0,  1, \
    /*  S */    1,  1,  1,  1,      0,  0,  0,  0, \
}

#endif

