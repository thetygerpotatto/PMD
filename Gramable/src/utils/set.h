#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "d_string.h"

#ifndef SET
#define SET

typedef struct {
    d_string * symbols;
    size_t size;
    size_t capacity;
} set;



void set_realloc(set *Set);

void set_append(set *Set, const char *c);

void set_clear(set *Set);

bool isInSet(set *Set, d_string *str);

int findInSet(set *set, d_string *str);
void set_delete(set *set, int index);

#endif // !Set
