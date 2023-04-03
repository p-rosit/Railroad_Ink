#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>

typedef struct list_element list_element_t;
typedef struct linked_list  linked_list_t;
typedef void (*free_func_t)(void*);

struct linked_list {
    size_t size;
    list_element_t* frst;
    list_element_t* last;
};

struct list_element {
    void* data;
    list_element_t* next;
};

linked_list_t* init_list();
void           append(linked_list_t*, void*);
void           free_list(linked_list_t*, free_func_t);

linked_list_t* init_list() {
    linked_list_t* list = malloc(sizeof(linked_list_t));
    list->size = 0;
    list->frst = NULL;
    list->last = NULL;

    return list;
}

void append(linked_list_t* list, void* data) {
    list_element_t* elm = malloc(sizeof(list_element_t));
    elm->data = data;
    elm->next = NULL;

    list->size += 1;
    list->last->next = elm;
    list->last = elm;
}

void free_list(linked_list_t* list, free_func_t func) {
    list_element_t *elm, *nxt;
    elm = list->frst;

    if (func != NULL) {
        for (int i = 0; i < list->size; ++i) {
            nxt = elm->next;
            func(elm->data);
            free(elm);
        }
    } else {
        for (int i = 0; i < list->size; ++i) {
            nxt = elm->next;
            free(elm);
        }
    }

    free(list);
}

#endif
