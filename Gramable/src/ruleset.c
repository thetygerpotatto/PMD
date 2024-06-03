#include "ruleset.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void realloc_ruleset(ruleset *rs) {
    size_t initial_cap = rs->capacity;
    rs->capacity+= 5 + 1;
    if (initial_cap == 0) rs->rules = calloc(rs->capacity, sizeof(rule));
    else rs->rules = realloc(rs->rules, rs->capacity*sizeof(rule));
}

void add_rule(ruleset *rs, rule r) {
    if (rs->size == rs->capacity - 1 || rs->size == 0) {
        realloc_ruleset(rs);
    }

    if (isInrs(rs, &r)) {
        return;
    }

    memcpy(&rs->rules[rs->size++], &r, sizeof(rule));
}

bool isInrs(ruleset *rs, rule * r) {
    if (rs == NULL) return false;
    
    for (size_t i = 0; i != rs->size; ++i) {
        if (strcmp(rs->rules[i].L.chars, r->L.chars) == 0 &&
            strcmp(rs->rules[i].R.chars, r->R.chars) == 0) return true;
    }
    return false;
}

void delete_rule(ruleset *rs, int index) {
    if (rs == NULL || index < 0 || index >= rs->size) {
        return;
    }
    
    free(rs->rules[index].R.chars);
    free(rs->rules[index].L.chars);
    rs->size--;
    
    for (size_t i = index; i < rs->size; ++i) {
        memcpy(&rs->rules[i], &rs->rules[i+1], sizeof(rule));
    }
}
