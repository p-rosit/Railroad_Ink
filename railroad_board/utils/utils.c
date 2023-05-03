#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define FILE_SEP            ('/')
#define MAX_LINE_LENGTH     (500)
#define OUTER_SCOPE         (0)

#define true  (1)
#define false (0)

typedef uint8_t     bool;
typedef char*       string;
typedef void        (*free_func_t)(void*);

enum debug_levels {
    DEBUG = 0,
    INFO  = 1,
    WARN  = 2,
    ERROR = 3,
    FATAL = 4
};

string         join_path(int nargs, ...);
string         copy_string(string);
bool           strstart(string, string);
bool           streq(string, string);

string join_path(int nargs, ...) {
    va_list valist;
    int i, j, k;
    string *strs, res;
    size_t total_size;

    strs = malloc(nargs * sizeof(string));
    va_start(valist, nargs);

    total_size = 0;
    for (i = 0; i < nargs; i++) {
        j = 0;
        strs[i] = va_arg(valist, string);

        while (strs[i][j] != '\0') j++;
        total_size += j + 1;
    }

    va_end(valist);

    k = 0;
    res = malloc(total_size * sizeof(char));
    for (i = 0; i < nargs; i++) {
        j = 0;
        while (strs[i][j] != '\0') {
            res[k++] = strs[i][j++];
        }
        res[k++] = FILE_SEP;
    }

    free(strs);
    res[k - 1] = '\0';
    return res;
}

string copy_str(string str) {
    size_t size;
    char c;
    string copy;

    size = 0;
    while ((c = str[size++]) != '\0');

    copy = malloc(size * sizeof(char));
    for (int i = 0; i < size; ++i) {
        copy[i] = str[i];
    }

    return copy;
}

string strip_while(string str, char c) {
    while (*str == c) str++;
    return str;
}

string strip_to(string str, char c) {
    char s;
    while ((s = *(str++)) != c && s != '\0');
    return str - (s == '\0');
}

string separate_name(string str) {
    char c;

    c = *str;
    while (isalpha(c) || c == '_') {
        c = *(++str);
    }
    *(str++) = '\0';
    return str;
}

string strconcat(string a, string b) {
    size_t len;
    int i, j;
    char c, *res;

    len = 0; i = 0; j = 0;
    while(a[i++] != '\0') len++;
    while(b[j++] != '\0') len++;
    len += 2;

    res = malloc(len * sizeof(char));

    i = 0; j = 0;
    while ((c = a[i++]) != '\0') res[j++] = c;
    res[j++] = '/';
    i = 0;
    while ((c = b[i++]) != '\0') res[j++] = c;
    res[j++] = '\0';

    return res;
}

bool strstart(string sub, string string) {
    int i;
    bool match;

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

bool streq(string a, string b) {
    return strcmp(a, b) == 0;
}

int parse_uint(string line) {
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
