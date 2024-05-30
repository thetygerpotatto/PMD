#ifndef RULESET
#define RULESET
#include "utils/d_string.h"
#include <stddef.h>
#include <stdbool.h>

typedef struct {
    d_string *R;
    d_string *L;
} rule;

typedef struct {
    rule * rules;
    size_t size;
    size_t capacity;
} ruleset;

void realloc_ruleset(ruleset *r);
void add_rule(ruleset * rs, rule r);
bool isInrs(ruleset *rs, rule * r);

#endif // DEBUG
