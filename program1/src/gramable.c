#include <stdio.h>
#include <stdbool.h>
#include "set.c"

bool isIn(const char *c, char needle);
void menu();
void getAlphabet();
void getTermSyms();
void getInitialSym();
void showGramatic();

static set Alphabet = {0};
static set Term_symbols = {0};
static char Inicial_symbol = 0;



int main() {
    menu();
}


void menu() {
    bool loop = false;
    while (!loop) {
        int op = -1;
        printf("      Bienvenido a Gramable\n\n");
        printf("1 - Para ingresar el vocabulario\n");
        printf("2 - Para ingresar los simbolos terminales\n");
        printf("3 - Para ingresar el simbolo inicial\n");
        printf("4 - Para mostrar la gramaticaa actual\n");
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
                showGramatic();
            break;
            case 0:
                loop = true;
            break;
        }
    }
}

void getAlphabet() {
    printf( "Ingrese su vocabulario\nIngrese una linea vacia para parar de ingresar caracteres:\n");
    while (true) {
        char sym = 0;
        sym = getchar();
        printf("%d", sym);

        if (sym == '\n') {
            printf("0");
            if (Alphabet.size == 0) continue;
            break;
        } else {
            printf("1");
            set_append(&Alphabet, sym);
            getchar();
        }
    }
}

void getTermSyms() {
    printf( "Ingrese sus Symbolos terminales\nIngrese una linea vacia para parar de ingresar caracteres:\n");
    while (true) {
        printf("- ");
        char sym = 0;
        scanf("%c", &sym);

        if (sym == '\n') {
            break;
        }

        if (!isIn(Alphabet.chars, sym)) {
            printf("Ingrese solo simbolos que esten en el vocabulario porfavor\n");
            getchar();
            continue;
        } 
        
        set_append(&Term_symbols, sym);
        getchar();
    }
}

void getInitialSym() {
    printf( "Ingrese el simbolo inicial\nIngrese una linea vacia para parar de ingresar caracteres:\n");
    while(true) {
        printf("- ");
        char sym;
        sym = getchar();
        if (!isIn(Alphabet.chars, sym)) {
            printf("Ingrese solo simbolos que esten en el vocabulario porfavor\n");
            getchar();
            continue;
        }
        if (isIn(Term_symbols.chars, sym)) {
            printf("El simbolo inicial no puede ser parte de los simbolos terminlaes\n");
            getchar();
            continue;
        }
        Inicial_symbol = sym;
        break;
    }
}

void showGramatic() {
    for (size_t i = 0; i != Alphabet.size; ++i) {
        if (i == 0) printf("V = {");
        if (i != Alphabet.size-1) printf("%c, ", Alphabet.chars[i]);
        else if (i == Alphabet.size-1 ) printf("%c}\t", Alphabet.chars[i]);
    }
    for (size_t i = 0; i != Term_symbols.size; ++i) {
        if (i == 0) printf("T = {");
        if (i != Term_symbols.size-1) printf("%c, ", Term_symbols.chars[i]);
        else if (i == Term_symbols.size-1 ) printf("%c}\t", Term_symbols.chars[i]);
    }

    printf("Simbolo inicial: %c\n", Inicial_symbol);
}
