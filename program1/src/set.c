#include "set.h"
#include <stdbool.h>

void set_realloc(set *str) {
    str->capacity += 5 + 1;
    str->chars = (char*)realloc(str->chars, str->capacity);
}

void set_append(set *str, const char c) {
    if (str->size == str->capacity - 1 || str->size == 0) {
        set_realloc(str);
    }      
    if (isIn(str->chars, c)) return;
    str->chars[str->size++] = c;
    str->chars[str->size] = 0;
} 

bool isIn(const char *list, char needle) {
    if (list == NULL) return  false;
    size_t lenght = strlen(list);
    if (lenght == 0) return false;
    for (size_t i = 0; i != lenght; ++i) {
        if (list[i] == needle) return true;
    }
    return false;
}

