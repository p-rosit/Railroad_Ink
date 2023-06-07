#define DEBUG_LEVEL (1)

#include "utils.c"
#include "linked_list.c"
#include "robin_hood.c"
#include "string_map.c"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    linked_list_t* list;
    string_map_t* map;

    list = init_list();
    append(list, copy_str("C"));
    append(list, copy_str("R"));
    append(list, copy_str("CC"));

    map = make_string_map_u16(10, 1000, list);

    printf("%s: %d\n", get_mapped_string(map, 0), get_mapped_u16(map, "C"));
    printf("%s: %d\n", get_mapped_string(map, 1), get_mapped_u16(map, "R"));
    printf("%s: %d\n", get_mapped_string(map, 2), get_mapped_u16(map, "CC"));


    free_list(list, free);
    free_string_map(map);
}
