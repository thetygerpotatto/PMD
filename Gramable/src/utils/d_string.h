
#ifndef DYNAMIC_STRING
#define DYNAMIC_STRING

#include <stddef.h>

typedef struct  {
    char * chars;
    size_t size;
    size_t capacity;

} d_string;


void d_string_reallocs(d_string *str, const char *);
void d_string_reallocc(d_string *str);

void d_string_append_s(d_string *str, const char *c); 
void d_string_append_c(d_string *str, const char c); 

void d_string_clear(d_string *str);

#endif // !DYNAMIC_STRING
