#include "set.h"
#include "d_string.h"
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

void set_realloc(set *set) {
    size_t inicial_cap = set->capacity;
    set->capacity += 5 + 1;

    if (inicial_cap == 0) set->symbols = (d_string *)malloc(sizeof(d_string)*set->capacity);

    else {
        d_string *syms = malloc(sizeof(d_string)*set->size);
        memcpy(syms, set->symbols, sizeof(d_string)*set->size);

        printf("size: %lu", set->capacity);
        
        set->symbols = (d_string *)realloc(set->symbols, sizeof(d_string)*set->capacity);

        memcpy(set->symbols, syms, sizeof(d_string)*set->size);
        free(syms);
    }
}

void set_append(set *Set, const char *c) {
    if (Set->size == Set->capacity - 1 || Set->size == 0) {
        set_realloc(Set);
    }

    d_string str = {0};
    d_string_append_s(&str, c);

    if (isInSet(Set, str)) {
        return ;
    }

    memcpy(&Set->symbols[Set->size++], &str, sizeof(d_string));
} 


void set_clear(set *set) {
    for (size_t i = 0; i != set->size ; ++i) {
        free(set->symbols[i].chars);
    }
    free(set->symbols);
    set->size = 0;
    set->capacity = 0;
}

bool isInSet(set *Set, d_string str) {
    if (Set == NULL) return  false;

    if (Set->size == 0) return false;
    for (size_t i = 0; i != Set->size; ++i) {
        if (strcmp(Set->symbols[i].chars, str.chars) == 0) return true;
    }
    return false;
}
