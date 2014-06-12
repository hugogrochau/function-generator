#include <stdio.h>
#include "gera_func.h"

/*
* Valores usados para teste
*/
#define INTEIRO1 0x01020304
#define DOUBLE2  -25.1234
#define CHAR2 'a'

static char str1[] = "* primeira * ";

/*
* Descritor de parâmetros
*/
static Parametro params[4];

/* Tipos das funções */
typedef int (*func_ptr_d_p) (double d, char *p);
typedef int (*func_ptr_d_c) (double d, char c);

/*
* Funções para teste de passagem de parâmetros
*/

/*
* Quatro parâmetros de tipos diferentes
*/
int quatro(int i, char *p, double d, char c) {
    printf("   => primeiro esperado = 0x%08x recebido = 0x%08x ", INTEIRO1, i);
    printf("\n   => segundo esperado = %s recebido = %s ", str1, p);
    printf("\n   => terceiro  esperado = %10.4lf recebido = %10.4lf ", DOUBLE2, d);
    printf("\n   => quarto esperado = %c recebido = %c ", CHAR2, c);

    return ((i == INTEIRO1) && (d == DOUBLE2) && (p == str1) && (c == CHAR2));
}

/*
* Bateria de testes
*/
int main (void) {
    func_ptr_d_p f_pointer_double;
    func_ptr_d_c f_double_char;

    /* Testa variações com quatro parâmetros */
    printf("\nTestes com quatro parâmetros");
    printf("\n----------------------------\n");

    printf("\nTestando: int amarrado, pointer variável (pos 2), double variável (pos 1), char amarrado\n");
    params[0].tipo = INT_PAR;
    params[0].amarrado = 1;
    params[0].valor.v_int = INTEIRO1;
    params[1].tipo = PTR_PAR;
    params[1].amarrado = 0;
    params[1].posicao = 2;
    params[2].tipo = DOUBLE_PAR;
    params[2].amarrado = 0;
    params[2].posicao = 1;
    params[3].tipo = CHAR_PAR;
    params[3].amarrado = 1;
    params[3].valor.v_char = CHAR2;

    f_pointer_double = (func_ptr_d_p)gera_func(quatro, 4, params);
    if (f_pointer_double(DOUBLE2, str1)) {
        printf(" -> OK\n");
    } else
        printf(" -> NÂO OK\n");
    libera_func(f_pointer_double);

    printf("\nTestando: int amarrado, pointer amarrado, double variável, char variavel\n");
    params[0].tipo = INT_PAR;
    params[0].amarrado = 1;
    params[0].valor.v_int = INTEIRO1;
    params[1].tipo = PTR_PAR;
    params[1].amarrado = 1;
    params[1].valor.v_ptr = str1;
    params[2].tipo = DOUBLE_PAR;
    params[2].amarrado = 0;
    params[2].posicao = 1;
    params[3].tipo = CHAR_PAR;
    params[3].amarrado = 0;
    params[3].posicao = 2;

    f_double_char = (func_ptr_d_c)gera_func(quatro, 4, params);
    if (f_double_char(DOUBLE2, CHAR2)) {
        printf(" -> OK\n");
    } else
        printf(" -> NÂO OK\n");

    libera_func(f_double_char);

    return 0;
}
