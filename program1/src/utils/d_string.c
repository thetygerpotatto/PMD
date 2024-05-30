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

void d_string_append_s(d_string *str, const char *c) {
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

void d_string_clear(d_string *str) {
    free(str->chars);
    str->size = 0;
    str->capacity = 0;
}
