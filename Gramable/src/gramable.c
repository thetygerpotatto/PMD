#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "ruleset.h"
#include "utils/set.c"
#include "utils/d_string.c"

void menu();
void getAlphabet();
void getTermSyms();
void getInitialSym();
void getRules();
void showGramatic();
void checkFrase();

bool isInSetc(set *Set, const char *c);

static set Alphabet = {0};
static set Term_symbols = {0};
static set Inicial_symbol = {0};

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
        printf("5 - Para mostrar la gramaticaa actual\n");
        printf("6 - Para probar una palabra\n");
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
                showGramatic();
            break;
            case 6:
                checkFrase();
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
        if (!isInSet(&Alphabet, &frase)) {
            printf("La frase contiene simbolos que no existen en el vocabulario V\n");
            return;
        }
    }

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
}

bool isInSetc(set *Set, const char *c) {
    for (size_t i = 0; i != Set->size; ++i) {
        if (strcmp(Set->symbols[i].chars, c) == 0) return true;
    }
    return false;
}
