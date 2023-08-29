#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdlib.h>

#define LINKED_LIST_FAILURE (0)
#define LINKED_LIST_SUCCESS (1)

#define MAKE_LINKED_LIST(type) \
typedef struct type##_list_element  type##_list_element_t; \
typedef struct type##_list          type##_list_t; \
 \
struct type##_list_element { \
    type##_list_element_t* next; \
    type data; \
}; \
 \
struct type##_list { \
    size_t length; \
    type##_list_element_t* frst; \
    type##_list_element_t* last; \
}; \
 \
type##_list_t type##_new_list() { \
    return (type##_list_t) { \
        .length = 0, \
        .frst = NULL, \
        .last = NULL \
    }; \
} \
 \
int type##_append_list_element(type##_list_t* list, type data) { \
    type##_list_element_t* elm; \
 \
    elm = malloc(sizeof(type##_list_element_t)); \
    if (elm == NULL) return LINKED_LIST_FAILURE; \
 \
    list->length += 1; \
    elm->next = NULL; \
    elm->data = data; \
 \
    if (list->frst == NULL) { \
        list->frst = elm; \
        list->last = elm; \
    } else { \
        list->last->next = elm; \
        list->last = elm; \
    } \
 \
    return LINKED_LIST_SUCCESS; \
} \
int type##_get_list_element(type##_list_t* list, size_t ind, type* data_ptr) { \
    size_t i; \
    type##_list_element_t* elm; \
 \
    i = 0; \
    elm = list->frst; \
    while (elm != NULL && i < ind) { \
        i += 1; \
        elm = elm->next; \
    } \
 \
    if (elm == NULL) return LINKED_LIST_FAILURE; \
    *data_ptr = elm->data; \
 \
    return LINKED_LIST_SUCCESS; \
} \
 \
int type##_remove_list_element(type##_list_t* list, size_t ind, type* data_ptr) { \
    size_t i; \
    type##_list_element_t *elm, *prev; \
 \
    i = 0; \
    prev = NULL; \
    elm = list->frst; \
    while (elm != NULL && i < ind) { \
        i += 1; \
        prev = elm; \
        elm = elm->next; \
    } \
 \
    if (elm == NULL) return LINKED_LIST_FAILURE; \
    if (data_ptr != NULL) *data_ptr = elm->data; \
 \
    if (prev == NULL) { \
        list->frst = elm->next; \
        free(elm); \
    } else { \
        prev->next = elm->next; \
        free(elm); \
    } \
 \
    return LINKED_LIST_SUCCESS; \
} \
 \
void type##_free_list(type##_list_t* list) { \
    type##_list_element_t *elm, *nelm; \
 \
    for (elm = list->frst; elm != NULL; elm = nelm) { \
        nelm = elm->next; \
        free(elm); \
    } \
 \
} \
 \

#define LIST_FOREACH(list, type, x, lambda) \
    do { \
        type##_list_element_t *_list_elm; \
        type x; \
        for (_list_elm = (list).frst; _list_elm != NULL; _list_elm = _list_elm->next) { \
            x = _list_elm->data; \
            lambda \
        } \
    } while (0);

#endif

