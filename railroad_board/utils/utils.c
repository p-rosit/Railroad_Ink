#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define FILE_SEP            ('/')
#define MAX_LINE_LENGTH     (500)
#define OUTER_SCOPE         (0)

#define true  (1)
#define false (0)

typedef uint8_t     bool_t;
typedef char*       string_t;
typedef void        (*free_func_t)(void*);

enum debug_levels {
    DEBUG = 0,
    INFO  = 1,
    WARN  = 2,
    ERROR = 3,
    FATAL = 4
};

string_t       join_path(int nargs, ...);
string_t       copy_string(string_t);
bool_t         strstart(string_t, string_t);
bool_t         streq(string_t, string_t);

string_t join_path(int nargs, ...) {
    va_list valist;
    int i, j, k;
    string_t *strings, res;
    size_t total_size;

    strings = (string_t*) malloc(nargs * sizeof(string_t));
    va_start(valist, nargs);

    total_size = 0;
    for (i = 0; i < nargs; i++) {
        j = 0;
        strings[i] = va_arg(valist, string_t);

        while (strings[i][j] != '\0') j++;
        total_size += j + 1;
    }

    va_end(valist);

    k = 0;
    res = (string_t) malloc(total_size * sizeof(char));
    for (i = 0; i < nargs; i++) {
        j = 0;
        while (strings[i][j] != '\0') {
            res[k++] = strings[i][j++];
        }
        res[k++] = FILE_SEP;
    }

    free(strings);
    res[k - 1] = '\0';
    return res;
}

string_t copy_string(string_t string) {
    size_t size;
    char c;
    string_t copy;

    size = 0;
    while ((c = string[size++]) != '\0');

    copy = (string_t) malloc(size * sizeof(char));
    for (int i = 0; i < size; ++i) {
        copy[i] = string[i];
    }

    return copy;
}

string_t strip_while(string_t string, char c) {
    while (*string == c) string++;
    return string;
}

string_t strip_to(string_t string, char c) {
    while (*(string++) != c);
    return string;
}

string_t strconcat(string_t a, string_t b) {
    size_t len;
    int i, j;
    char c, *res;

    len = 0; i = 0; j = 0;
    while(a[i++] != '\0') len++;
    while(b[j++] != '\0') len++;
    len += 2;

    res = (char*) malloc(len * sizeof(char));

    i = 0; j = 0;
    while ((c = a[i++]) != '\0') res[j++] = c;
    res[j++] = '/';
    i = 0;
    while ((c = b[i++]) != '\0') res[j++] = c;
    res[j++] = '\0';

    return res;
}

bool_t strstart(string_t sub, string_t string) {
    int i;
    bool_t match;

    i = 0;
    match = true;
    while (sub[i] != '\0') {
        if (sub[i] != string[i]) {
            match = false;
            break;
        }
        i++;
    }

    return match;
}

bool_t streq(string_t a, string_t b) {
    return strcmp(a, b) == 0;
}

int parse_uint(string_t line) {
    char c;
    int num = 0;

    c = *(line++);
    while (c != ' ' && c != '#' && c != '\n' && c != '\0') {
        if (c < '0' || '9' < c) {
            printf("Parse error: Expected digits when parsing unsigned int, got %c.\n", c);
            exit(1);
        }
        num = 10 * num + c - '0';
        c = *(line++);
    }

    return num;
}

#endif
