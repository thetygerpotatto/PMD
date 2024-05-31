#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "ruleset.c"
#include "ruleset.h"
#include "utils/d_string.c"
#include "utils/d_string.h"
#include "utils/set.c"

void menu();
void getAlphabet();
void getTermSyms();
void getInitialSym();
void getRules();
void showGramatic();
void checkFrase();
void generateLanguage();

bool isInSetc(set *Set, const char *c);
bool isComformedByVoc(set *Set, d_string *str);
bool isOnlyTerminal(set *TSet, d_string *str);
char * analize_word(char * word, set *Language, size_t depth);
char * lookForRecursion(rule * r);

static set Alphabet = {0};
static set Term_symbols = {0};
static set Inicial_symbol = {0};
static set GeneratedLanguage = {0};

static ruleset Rules = {0};

static d_string frase = {0};



int main() {
    menu();
}


void menu() {
    bool loop = false;
    while (!loop) {
        int op = -1;
        showGramatic();
        printf("\n      Bienvenido a Gramable\n\n");
        printf("1 - Para ingresar el vocabulario\n");
        printf("2 - Para ingresar los simbolos terminales\n");
        printf("3 - Para ingresar el simbolo inicial\n");
        printf("4 - Para ingresar las reglas\n");
        printf("5 - Para probar una palabra\n");
        printf("6 - Para generar el lenguaje\n");
        printf("0 - Para salir\n");
        printf("--");
        
        scanf("%d", &op);
        getchar();

        switch (op) {
            case 1:
                getAlphabet();
            break;
            case 2:
                getTermSyms();
            break;
            case 3:
                getInitialSym();
            break;
            case 4:
                getRules();
            break;
            case 5:
                checkFrase();
            break;
            case 6:
                generateLanguage();
            break;
            case 0:
                loop = true;
            break;
        }
    }
}

void checkFrase() {
    d_string_clear(&frase);
    char buffer[100];
    printf("Ingrese la palabra que desea comprobar: ");
    scanf("%s", buffer);
    getchar();

    d_string_append_s(&frase, buffer);
    
    for (size_t i = 0; i != frase.size; ++i) {
        if (!isComformedByVoc(&Alphabet, &frase)) {
            printf("La frase contiene simbolos que no existen en el vocabulario V\n");
            return;
        }
    }


}


void generateLanguage() {
    set generated_words = {0};
    d_string starting_sym = {0};

    d_string_append_s(&starting_sym, Inicial_symbol.symbols->chars);
    
    analize_word(starting_sym.chars, &GeneratedLanguage, 0);
    printf("Palabras generadas por la gramatica: \n");
    for (size_t i = 0; i < GeneratedLanguage.size; ++i) {
        printf("%lu - %s", i, GeneratedLanguage.symbols[i].chars);
        if (d_string_isIn_s(&GeneratedLanguage.symbols[i], "(")) {
            printf(" Para cualquier S de P que no se contenga a si misma\n");
        } else printf("\n");
    }
    getchar();
}

char * analize_word(char * word, set *Language, size_t depth) {
    depth++;
    d_string starting_sym = {0};
    d_string temp = {0};
    d_string_append_s(&starting_sym, word);
    
    for (size_t rule_Index = 0; rule_Index != Rules.size; ++rule_Index) {
        for (size_t str_Index = 0; str_Index != starting_sym.size; ++str_Index) {
            char str_ = starting_sym.chars[str_Index];
            char rule_ = *Rules.rules[rule_Index].L.chars;
            if (strncmp(&starting_sym.chars[str_Index], Rules.rules[rule_Index].L.chars, Rules.rules[rule_Index].L.size) == 0) {

                d_string_set_s(&temp, starting_sym.chars);

                d_string_delete_s(&temp, str_Index, Rules.rules[rule_Index].L.chars);
                d_string_insert_s(&temp, str_Index, Rules.rules[rule_Index].R.chars);
                
                d_string recursionRule = {0};
                d_string_append_s(&recursionRule, lookForRecursion(&Rules.rules[rule_Index]));

                bool hasRecursion = (recursionRule.chars == NULL) ? false : true;
                
                if (!isOnlyTerminal(&Term_symbols, &temp) && depth && !hasRecursion) {
                    analize_word(temp.chars, Language, depth);
                } 
                else if(depth > 1000) {
                    printf("Limite de recursion alcanzado%lu\n", depth);
                }
                else if(hasRecursion) {
                    set_append(&GeneratedLanguage, recursionRule.chars);
                }
                else {
                    set_append(&GeneratedLanguage, temp.chars);
                }
            }
            
        }
    }


    return temp.chars;
}

void getAlphabet() {
    printf( "Ingrese su vocabulario\nIngrese un '-' para parar de ingresar caracteres:\n");
    while (true) {
        char buffer[80];
        printf("- ");
        scanf("%s", buffer);

        if (strcmp(buffer, "-") == 0) {
            if (Alphabet.size == 0) continue;
            getchar();
            break;
        } else {
            set_append(&Alphabet, buffer);
            getchar();
        }
    }
}

void getTermSyms() {
    printf( "Ingrese sus Symbolos terminales\nIngrese un '-' para parar de ingresar caracteres:\n");
    while (true) {
        char buffer[80];
        printf("- ");
        scanf("%s", buffer);

        if (strcmp(buffer, "-") == 0) {
            break;
        }

        if (!isInSetc(&Alphabet, buffer)) {
            printf("Solo puede ingresar symbolos que pertenezcan al alfabeto\n");
            getchar();
        }
        
        if (isInSetc(&Inicial_symbol, buffer)) {
            printf("Un symbolo terminal no puede ser igual al simbolo inicial");
            getchar();
        }

        
        set_append(&Term_symbols, buffer);
        getchar();
    }
}

void getInitialSym() {
    printf( "Ingrese el simbolo inicial:");
    while(true) {
        printf("- ");
        char buffer[80];
        scanf("%s", buffer);

        if (!isInSetc(&Alphabet, buffer)) {
            printf("Ingrese solo simbolos que esten en el vocabulario porfavor\n");
            getchar();
            continue;
        }
        if (!isInSetc(&Alphabet, buffer)) {
            printf("'-' no puede ser un simbolo");
            getchar();
            continue;
        }
        if (isInSetc(&Term_symbols, buffer)) {
            printf("El simbolo inicial no puede ser parte de los simbolos terminlaes\n");
            getchar();
            continue;
        }

        set_append(&Inicial_symbol, buffer);
        getchar();
        break;
    }
}

void getRules() {
    printf( "Ingrese sus reglas P en pares \nIngrese un '-' para parar de ingresar caracteres:\n");
    while (true) {
        char bufferL[80];
        char bufferR[80];
        printf("- ");
        scanf("%s", bufferL);

        if (strcmp(bufferL, "-") == 0) {
            if (Alphabet.size == 0) continue;
            getchar();
            break;
        } 

        d_string strL = {0};
        d_string_append_s(&strL, bufferL);
        
        printf("isOnlyTerminal :%d\n", isOnlyTerminal(&Term_symbols, &strL));
        printf("isComformedByVoc :%d\n", isComformedByVoc(&Alphabet, &strL));
        if (isOnlyTerminal(&Term_symbols, &strL) || !isComformedByVoc(&Alphabet, &strL)) {
            printf("El simbolo del lado izquierdo debe estar en el vocabulario y no puede ser un simbolo terminal\n");
            getchar();
            continue;
        }

        printf("%s -> ", bufferL);
        scanf("%s", bufferR);

        if (strcmp(bufferL, "-") == 0) {
            if (Alphabet.size == 0) continue;
            getchar();
            break;
        } 

        d_string strR = {0};
        d_string_append_s(&strR, bufferR);
        
        if (!isComformedByVoc(&Alphabet, &strR)) {
            printf("El simbolo del lado derecho debe estar en el vocabulario\n");
            getchar();
            continue;
        }


        rule newrule = {.R = strR, .L = strL};
        add_rule(&Rules, newrule);
    }
}

void showGramatic() {
    for (size_t i = 0; i != Alphabet.size; ++i) {
        if (i == 0) printf("V = {");
        if (i != Alphabet.size-1) printf("%s, ", Alphabet.symbols[i].chars);
        else if (i == Alphabet.size-1 ) printf("%s}\t", Alphabet.symbols[i].chars);
    }
    for (size_t i = 0; i != Term_symbols.size; ++i) {
        if (i == 0) printf("T = {");
        if (i != Term_symbols.size-1) printf("%s, ", Term_symbols.symbols[i].chars);
        else if (i == Term_symbols.size-1 ) printf("%s}\t", Term_symbols.symbols[i].chars);
    }

    if (Inicial_symbol.size != 0) printf("Simbolo inicial: %s\n", Inicial_symbol.symbols->chars);

    for (size_t i = 0; i != Rules.size; ++i) {
        if (i == 0) printf("P = {");
        if (i != Rules.size-1) printf("%s -> %s, ", Rules.rules[i].L.chars, Rules.rules[i].R.chars);
        else if (i == Rules.size-1 ) printf("%s -> %s}\n", Rules.rules[i].L.chars, Rules.rules[i].R.chars);
    }
}

bool isInSetc(set *Set, const char *c) {
    for (size_t i = 0; i != Set->size; ++i) {
        if (strcmp(Set->symbols[i].chars, c) == 0) return true;
    }
    return false;
}

bool isComformedByVoc(set *VSet, d_string *str) {
    if (VSet == NULL) return  false;

    if (VSet->size == 0) return false;
    for (size_t str_Index = 0; str_Index != str->size; ++str_Index) {
        bool isInVoc = false;
        for (size_t Vsym_index = 0; Vsym_index != VSet->size; ++Vsym_index) {
            if (str->chars[str_Index] == *VSet->symbols[Vsym_index].chars) isInVoc = true;
        }
        if (!isInVoc) return false;
    }
    return true;
}

bool isOnlyTerminal(set *TSet, d_string *str) {
    if (TSet == NULL) return  false;
    
    for (size_t str_Index = 0; str_Index != str->size; ++str_Index) {
        bool onlyTerminal = false;

        for (size_t Tsym_index = 0; Tsym_index != TSet->size; ++Tsym_index) {
            if (str->chars[str_Index] == *TSet->symbols[Tsym_index].chars) {
                onlyTerminal = true;
            }
        }

        if (!onlyTerminal) return false;
    }

    return true;
}

char * lookForRecursion(rule * r) {
    d_string lg = {0};

    if (d_string_isIn_s(&r->R, r->L.chars)) {
        if (strncmp(r->L.chars, r->R.chars, r->L.size) == 0) {
            d_string_append_s(&lg, r->L.chars);
            d_string_append_s(&lg, "(");
            d_string_append_s(&lg, &r->R.chars[r->L.size]);
            d_string_append_s(&lg, ")*");
        } 
        else if (strncmp(r->L.chars, &r->R.chars[r->R.size - r->L.size], r->L.size) == 0) {
            d_string_append_s(&lg, "(");
            d_string_append_s(&lg, r->R.chars);
            d_string_delete_s(&lg, lg.size-r->L.size, r->L.chars);
            d_string_append_s(&lg, ")*");
            d_string_append_s(&lg, r->L.chars) ;
        } 
        else {
            size_t L_index = d_string_find_s(&r->R, r->L.chars);
            d_string_append_s(&lg, "(");
            d_string_concat_s(&lg, r->R.chars, L_index);
            d_string_append_s(&lg, ")*");
            d_string_append_s(&lg, r->L.chars);
            d_string_append_s(&lg, "(");
            d_string_concat_s(&lg, &r->R.chars[L_index+1], L_index);
            d_string_append_s(&lg, ")*");
        }
        return lg.chars;
    }
    return NULL;
}
