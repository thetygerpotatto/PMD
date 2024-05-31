
#ifndef DYNAMIC_STRING
#define DYNAMIC_STRING

#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>

typedef struct  {
    char * chars;
    size_t size;
    size_t capacity;

} d_string;


void d_string_reallocs(d_string *str, const char *);
void d_string_reallocc(d_string *str);
void d_string_reallocn(d_string *str, size_t n);

void d_string_append_s(d_string *str, const char *c); 
void d_string_append_c(d_string *str, const char c); 

void d_string_set_s(d_string *str, const char *c);

void d_string_delete_c(d_string *str, size_t index);
void d_string_delete_s(d_string *str, size_t index, const char *c);

void d_string_insert_s(d_string *str, size_t index, const char *c);

void d_string_concat_s(d_string *str, const char *c, size_t n);

void d_string_clear(d_string *str);

bool d_string_isIn_s(d_string *str, const char *c);
bool d_string_isIn_c(d_string *str, const char c);

int d_string_find_s(d_string *str, const char *c);


#endif // !DYNAMIC_STRING
