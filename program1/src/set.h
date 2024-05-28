#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#ifndef DYNAMIC_STRING
#define DYNAMIC_STRING 

typedef struct {
    char * chars;
    size_t size;
    size_t capacity;
} set;



void set_realloc(set *str);

void set_append_c(set *str, const char c); 

bool isIn(const char *list, char needle);

#endif // !Dynamic_String
