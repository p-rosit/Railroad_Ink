#include "hash_map.c"
#define DEBUG_LEVEL (1)

#include "utils.c"
#include "linked_list.c"
#include "hash_map.c"
#include "string_map.c"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef uint8_t u8;

MAKE_LINKED_LIST(string);
MAKE_HASH_MAP(u8);
MAKE_STRING_MAP(u8);

int main() {
    string str;
    string_list_t list;
    u8_string_map_t map;

    list = string_new_list();
    string_append_list_element(&list, copy_str("C"));
    string_append_list_element(&list, copy_str("R"));
    string_append_list_element(&list, copy_str("CC"));

    map = new_u8_string_map(&list, 10, 1000);

    printf("%s: %d\n", get_u82string(&map, 0), get_string2u8(&map, "C"));
    printf("%s: %d\n", get_u82string(&map, 1), get_string2u8(&map, "R"));
    printf("%s: %d\n", get_u82string(&map, 2), get_string2u8(&map, "CC"));
    /*
    map = make_string_map_u8(10, 1000, list);

    printf("%s: %d\n", get_mapped_string(map, 0), get_mapped_u16(map, "C"));
    printf("%s: %d\n", get_mapped_string(map, 1), get_mapped_u16(map, "R"));
    printf("%s: %d\n", get_mapped_string(map, 2), get_mapped_u16(map, "CC"));


    free_list(list, free);
    free_string_map(map);
*/
    free_u8_string_map(&map);
    LIST_FOREACH(list, string, str, {
        free(str);
    })
    string_free_list(&list);
}
