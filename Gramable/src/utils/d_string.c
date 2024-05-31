#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "d_string.h"

void d_string_reallocs(d_string *str, const char *c) {
    size_t inicial_cap = str->capacity;
    str->capacity += strlen(c) + 5 + 1;
    if (inicial_cap == 0) str->chars = (char *)malloc(str->capacity);
    else str->chars = (char*)realloc(str->chars, str->capacity);
}

void d_string_reallocc(d_string *str) {
    size_t inicial_cap = str->capacity;
    str->capacity += 5 + 1;
    if (inicial_cap == 0) str->chars = (char *)malloc(str->capacity);
    else str->chars = (char*)realloc(str->chars, str->capacity);
}

void d_string_reallocn(d_string *str, size_t n) {
    size_t inicial_cap = str->capacity;
    str->capacity += n + 5 + 1;
    if (inicial_cap == 0) str->chars = (char *)malloc(str->capacity);
    else str->chars = (char*)realloc(str->chars, str->capacity);
}

void d_string_append_s(d_string *str, const char *c) {
    if (c == NULL) return;
    size_t length = strlen(c);
    if (str->size + length >= str->capacity - 1 || str->size == 0) {
        d_string_reallocs(str, c);
    }      

    for (size_t i = 0 ; i != length; i++) {
        str->chars[str->size++] = c[i];
        str->chars[str->size] = 0;
    }
} 

void d_string_append_c(d_string *str, const char c){
    if (str->size == str->capacity - 1 || str->size == 0) {
        d_string_reallocc(str);
    }      
    str->chars[str->size++] = c;
    str->chars[str->size] = 0;
}

void d_string_delete_c(d_string *str, size_t index) {
    for (size_t i = index; i != str->size; ++i) {
        str->chars[i] = str->chars[i+1];
    }
    str->size--;
}

void d_string_delete_s(d_string *str, size_t index, const char *c) {
    size_t length = strlen(c);
    for (size_t i = index; i != str->size; ++i) {
        str->chars[i] = str->chars[i+length];
    }
    str->size-=length;
}

void d_string_insert_s(d_string *str, size_t index, const char *c) {
    size_t insert_lenght = strlen(c);
    if (str->size + insert_lenght >= str->capacity) {
        d_string_reallocs(str, c);
    }
    
    for (size_t i = str->size; i != index-1; --i) {
        str->chars[i+insert_lenght] = str->chars[i];
    }
    
    for (size_t i = index; i != index+insert_lenght; ++i) {
        str->chars[i] = c[i-index];
    }
    str->size+=insert_lenght;
}

void d_string_set_s(d_string *str, const char *c) {
    d_string_clear(str);
    d_string_append_s(str, c);
}

void d_string_clear(d_string *str) {
    free(str->chars);
    str->size = 0;
    str->capacity = 0;
}

bool d_string_isIn_s(d_string *str, const char *c) {
    if (str == NULL || c == NULL) return false;
    size_t length = strlen(c);

    for (size_t str_Index = 0; str_Index != str->size; ++str_Index) {
        if (strncmp(&str->chars[str_Index], c, length) == 0) return true;
    }

    return false;
}

bool d_string_isIn_c(d_string *str, const char c);

void d_string_concat_s(d_string *str, const char *c, size_t n) {
    if (str == NULL || c == NULL) return;
    if (str->capacity-1 >= str->size+n) {
        d_string_reallocn(str, n);
    }

    for (size_t i = 0; i != n; ++i) {
        d_string_append_c(str, c[i]);
    }
}

int d_string_find_s(d_string *str, const char *c) {
    int length = strlen(c);
    for (int i = 0; i != str->size; ++i) {
        if (strncmp(&str->chars[i], c, length) == 0) {
            return i;
        }
    }

    return -1;
}
