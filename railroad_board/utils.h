#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>

typedef struct list_element list_element_t;
typedef struct linked_list  linked_list_t;
typedef struct array        array_t;
typedef struct dictionary   dictionary_t;
typedef void   (*free_func_t)(void*);

struct linked_list {
    size_t size;
    list_element_t* frst;
    list_element_t* last;
};

struct list_element {
    void* data;
    list_element_t* next;
};

struct array {
    size_t size;
    void** data;
};

struct dictionary {
    size_t max_size;
    size_t size;
    size_t* inds;
    size_t key_amount;
    uint8_t* vals;
};

char*          copy_string(char*);

linked_list_t* init_list();
void           append(linked_list_t*, void*);
void           free_list(linked_list_t*, free_func_t);

array_t*       init_array();
void           free_array(array_t*);

dictionary_t*  init_dictionary(size_t);
size_t         hash_string(char*);
void           add_num(dictionary_t*, char*, uint8_t);
void           add_key(dictionary_t*, size_t);
void           increase_dictionary_size(dictionary_t*, size_t, uint8_t);
void           free_dictionary(dictionary_t*);

char* copy_string(char* string) {
    size_t size;
    char c, *traverse, *copy;

    size = 0;
    while ((c = string[size++]) != '\0');

    copy = malloc(size * sizeof(char));
    for (int i = 0; i < size; ++i) {
        copy[i] = string[i];
    }

    return copy;
}

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

array_t* init_array() {
    array_t* arr = malloc(sizeof(array_t));
    arr->size = 0;
    arr->data = NULL;
}

void free_array(array_t* arr) {
    free(arr);
}

dictionary_t* init_dictionary(size_t max_size) {
    dictionary_t* dict = malloc(sizeof(dictionary_t));
    dict->max_size = max_size;
    dict->size = 1;

    dict->key_amount = 0;
    dict->inds = NULL;
    
    dict->vals = malloc(1 * sizeof(uint8_t));
    dict->vals[0] = 255;

    return dict;
}

size_t hash_string(char* key) {
    size_t hash;
    int c;

    hash = 5381;
    while ((c = *key++) != '\0') {
        hash = ((hash << 5) + hash) + c;
    }

    return hash;
}

void add_num(dictionary_t* dict, char* key, uint8_t num) {
    size_t ind;

    if (num == 255) {
        printf("Tried to insert value 255 into hash table. Invalid value.\n");
        exit(1);
    }

    ind = hash_string(key);
    if (dict->vals[ind % dict->size] != 255) {
        increase_dictionary_size(dict, ind, num);
    }

    add_key(dict, ind);
    dict->vals[ind % dict->size] = num;
}

void add_key(dictionary_t* dict, size_t ind) {
    size_t* new_inds = realloc(dict->inds, (dict->key_amount + 1) * sizeof(size_t));

    if (new_inds != NULL) {
        new_inds[dict->key_amount] = ind;
    } else {
        new_inds = malloc((dict->key_amount + 1) * sizeof(size_t));
        for (int i = 0; i < dict->key_amount; ++i) {
            new_inds[i] = dict->inds[i];
        }
        new_inds[dict->key_amount] = ind;
        free(dict->inds);
    }

    dict->key_amount += 1;
    dict->inds = new_inds;
}

void increase_dictionary_size(dictionary_t* dict, size_t ind, uint8_t num) {
    size_t new_size;
    uint8_t* new_vals;
    
    new_size = dict->size;
    new_vals = dict->vals;
    while (new_size < dict->max_size) {
        new_size += 1;
        new_vals = malloc(new_size * sizeof(uint8_t));

        for (int i = 0; i < new_size; ++i) {
            new_vals[i] = 255;
        }

        for (int i = 0; i < dict->key_amount; ++i) {
            if (new_vals[dict->inds[i] % new_size] != 255) {
                goto next_size;
            } else {
                new_vals[dict->inds[i] % new_size] = dict->vals[dict->inds[i] % dict->size];
            }
        }

        if (new_vals[ind % new_size] == 255) {
            break;
        }

        next_size:
        free(new_vals);
    }

    if (dict->max_size <= new_size) {
        printf("Dictionary tried to grow past %lu.\n", dict->max_size);
        exit(1);
    }

    free(dict->vals);

    dict->size = new_size;
    dict->vals = new_vals;
}

void free_dictionary(dictionary_t* dict) {
    free(dict->vals);
    free(dict->inds);
    free(dict);
}

#endif
