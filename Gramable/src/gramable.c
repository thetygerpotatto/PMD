#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "ruleset.c"
#include "ruleset.h"
#include "utils/d_string.c"
#include "utils/d_string.h"
#include "utils/set.c"

#define RECURSION_LIMIT 1000
#define BRANCH_LIMIT 10000

void menu();
void getAlphabet();
void getTermSyms();
void getInitialSym();
void getRules();
void showGramatic();
void checkFrase();
void generateLanguage();
void editingMenu();
void editAlphabet();
void editTerminalSymbols();
void editInicialSymbol();
void editRules();

bool isInSetc(set *Set, const char *c);
bool isComformedByVoc(set *Set, d_string *str);
bool isOnlyTerminal(set *TSet, d_string *str);
char * derive_frase(char * word, set *Language, size_t depth);
bool analize_frase(char * frase);
char * lookForRecursion(rule * r);

static set Alphabet = {0};
static set Term_symbols = {0};
static set Inicial_symbol = {0};
static set Generated_language = {0};

static ruleset Rules = {0};

static d_string frase = {0};

static bool recursion_limit_reached = false;
static size_t branch_limit = 0;




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
        printf("7 - Menu de edicion\n");
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
            case 7:
                editingMenu();
            break;
            case 0:
                loop = true;
            break;
        }
    }
}

void editingMenu() {
    bool loop = true;
    while (loop) {
        int op = -1;
        showGramatic();
        printf("\n      menu de edicion\n\n");
        printf("1 - Para editar el vocabulario\n");
        printf("2 - Para editar los simbolos terminales\n");
        printf("3 - Para editar el simbolo inicial\n");
        printf("4 - Para editar las reglas\n");
        printf("0 - Para salir\n");
        printf("--");
        
        scanf("%d", &op);
        getchar();

        switch (op) {
            case 1:
                editAlphabet();
            break;
            case 2:
                editTerminalSymbols();
            break;
            case 3:
                editInicialSymbol();
            break;
            case 4:
                editRules();
            break;
            case 0:
                loop = false;
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
    
    if (!isComformedByVoc(&Alphabet, &frase)) {
        printf("La frase contiene simbolos que no existen en el vocabulario V\n");
        return;
    }
    if (!isOnlyTerminal(&Term_symbols, &frase)) {
        printf("La frase contiene simbolos no terminales\n");
        return;
    }

    if (analize_frase(frase.chars)) {
        printf("%s es una frase de generada por G\n", frase.chars);
    } else {
        printf("%s no es una frase de generada por G\n", frase.chars);
    }
    getchar();
}


void generateLanguage() {
    if (Rules.rules == NULL) return;
    d_string starting_sym = {0};

    d_string_append_s(&starting_sym, Inicial_symbol.symbols->chars);
    
    derive_frase(starting_sym.chars, &Generated_language, 0);
    printf("Palabras generadas por la gramatica: \n");
    for (size_t i = 0; i < Generated_language.size; ++i) {
        printf("%lu - %s", i+1, Generated_language.symbols[i].chars);
        if (d_string_isIn_s(&Generated_language.symbols[i], "(")) {

            printf("--> el simbolo no terminal fuera de parentesis sera remplazado por cualquier regla que no lo contenga en su lado derecho\n");
        } else printf("\n");
    }
    if (recursion_limit_reached) {
        recursion_limit_reached = false;
        printf("El limite de rercurcion fue alcanzado - Limite de recurcion: %d \nAsegurece de que su Gramatica no contenga un bucle\n",RECURSION_LIMIT);
    }
    if (branch_limit == BRANCH_LIMIT) {

        printf("El limite de generacion de ramas fue alcanzado - limite de genreacion de ramas: %d \nAsegurece de que su Gramatica no contenga un bucle\n",BRANCH_LIMIT);
    }
    getchar();
}

char * derive_frase(char * word, set *Language, size_t depth) {
    branch_limit++;
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
                char * dump = lookForRecursion(&Rules.rules[rule_Index]);
                d_string_append_s(&recursionRule, dump);
                if (dump != NULL) free(dump);

                bool hasRecursion = (recursionRule.chars == NULL) ? false : true;
                
                if (!isOnlyTerminal(&Term_symbols, &temp) &&
                    depth < RECURSION_LIMIT &&
                    !hasRecursion &&
                    branch_limit < BRANCH_LIMIT) {
                    derive_frase(temp.chars, Language, depth);
                } 
                else if(depth >= RECURSION_LIMIT) {
                    recursion_limit_reached = true;
                }
                else if(hasRecursion) {
                    set_append(&Generated_language, recursionRule.chars);
                }
                else {
                    if (isComformedByVoc(&Term_symbols, &temp)) set_append(&Generated_language, temp.chars);
                }
            }
            
        }
    }


    return temp.chars;
}

bool analize_frase(char * frase) {
    d_string inicial_frase = {0};
    d_string temp = {0};
    d_string_append_s(&inicial_frase, frase);
    bool isIn = false;

    
    for (size_t rule_Index = 0; rule_Index != Rules.size; ++rule_Index) {
        for (size_t str_Index = 0; str_Index != inicial_frase.size; ++str_Index) {
            char str_ = inicial_frase.chars[str_Index];
            char rule_ = *Rules.rules[rule_Index].R.chars;
            if (strncmp(&inicial_frase.chars[str_Index], Rules.rules[rule_Index].R.chars, Rules.rules[rule_Index].R.size) == 0) {

                d_string_set_s(&temp, inicial_frase.chars);

                d_string_delete_s(&temp, str_Index, Rules.rules[rule_Index].R.chars);
                d_string_insert_s(&temp, str_Index, Rules.rules[rule_Index].L.chars);
                

                if (!d_string_isIn_s(&temp, Inicial_symbol.symbols->chars) || temp.size > 1) {
                    bool istemp = analize_frase(temp.chars);
                    if (!isIn) isIn = istemp;
                } else {
                    isIn = true;
                }
            }
        }
    }


    return isIn;
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

void editAlphabet() {
    bool loop = true;
    int index = -2;
    d_string deleted = {0};
    while (loop) {
        printf("Vocabulario:\n");
        for (size_t i = 0; i != Alphabet.size; ++i) {
            printf("%lu - %s\n", i+1, Alphabet.symbols[i].chars);
        }
        printf("Seleccione el indice del simbolo que desea eliminar (0 para cancelar): ");
        scanf("%d", &index);
        getchar();
        index--;
        
        if (index == -1) {
            break;
        }

        if (index < 0 && index >= Alphabet.size ) {
            printf("Ingrese indices dentro del rango\n");
            continue;
        }
        d_string_set_s(&deleted, Alphabet.symbols[index].chars);
        set_delete(&Alphabet, index);
    }

    set_delete(&Inicial_symbol, findInSet(&Inicial_symbol, &deleted));
    set_delete(&Term_symbols, findInSet(&Term_symbols, &deleted));
    
    for (int i = 0; i < Rules.size; ++i) {
        if (d_string_isIn_s(&Rules.rules[i].L, deleted.chars) || 
            d_string_isIn_s(&Rules.rules[i].R, deleted.chars)) {
            delete_rule(&Rules, i);
        }
    }
}

void editTerminalSymbols() {
    bool loop = true;
    int index = -2;
    d_string deleted = {0};
    while (loop) {
        printf("Simbolos terminales:\n");
        for (size_t i = 0; i != Term_symbols.size; ++i) {
            printf("%lu - %s\n", i+1, Term_symbols.symbols[i].chars);
        }
        printf("Seleccione el indice del simbolo que desea eliminar (0 para cancelar): ");
        scanf("%d", &index);
        getchar();
        index--;
        
        if (index == -1) {
            break;
        }

        if (index < 0 && index >= Term_symbols.size ) {
            printf("Ingrese indices dentro del rango\n");
            continue;
        }
        d_string_set_s(&deleted, Term_symbols.symbols[index].chars);
        set_delete(&Term_symbols, index);
    }

    for (int i = 0; i < Rules.size; ++i) {
        if (d_string_isIn_s(&Rules.rules[i].L, deleted.chars) || 
            d_string_isIn_s(&Rules.rules[i].R, deleted.chars)) {
            delete_rule(&Rules, i);
        }
    }
}

void getTermSyms() {
    if (Alphabet.symbols == NULL) return;
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
    if (Alphabet.symbols == NULL || Inicial_symbol.size > 0) return;
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

void editInicialSymbol() {
    bool loop = true;
    char index = 0;
    d_string deleted = {0};
    while (loop) {
        printf("Simbolo inical:%s\n", Inicial_symbol.symbols->chars);
        printf("Ingrese 0 para cancelar, cualquier otra tecla para continuar");
        scanf("%c", &index);
        getchar();
        
        if (index == '0') {
            break;
        }


        set_delete(&Inicial_symbol, 0);
        break;
    }

    if (index == '0') return;


    getInitialSym();
}

void getRules() {
    if (Alphabet.symbols == NULL) return;
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
        
        //printf("isOnlyTerminal :%d\n", isOnlyTerminal(&Term_symbols, &strL));
        //printf("isComformedByVoc :%d\n", isComformedByVoc(&Alphabet, &strL));
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

void editRules() {
    bool loop = true;
    int index = -2;
    while (loop) {
        printf("Reglas de produccion\n");
        for (size_t i = 0; i != Rules.size; ++i) {
            printf("%lu - %s -> %s\n", i+1, Rules.rules[i].L.chars, Rules.rules[i].R.chars);
        }
        printf("Seleccione el indice de la regla que desea eliminar (0 para cancelar): ");
        scanf("%d", &index);
        getchar();
        index--;
        
        if (index == -1) {
            break;
        }

        if (index < 0 && index >= Rules.size ) {
            printf("Ingrese indices dentro del rango\n");
            continue;
        }
        delete_rule(&Rules, index);
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
