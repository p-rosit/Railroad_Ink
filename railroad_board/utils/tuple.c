#ifndef TUPLE_H
#define TUPLE_H

#include <stdlib.h>

typedef struct tuple2 tuple2_t;

tuple2_t* init_tuple2();

struct tuple2 {
    void* data1;
    void* data2;
};

tuple2_t* init_tuple2() {
    tuple2_t* tup = malloc(sizeof(tuple2_t));
    return tup;
}

#endif
