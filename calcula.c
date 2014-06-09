#include <stdio.h>
#include <string.h>
#include "gera_func.h"

double calcula(int a, double b, double c) {
    return a + b - c;
}

typedef double (*func_ptr) (int a, double b, double c);

int main(void) {
    Parametro params[3];
    func_ptr foo = NULL;
    int a;
    double b, c;

    params[0].tipo = INT_PAR;
    params[0].amarrado = 0;
    params[0].posicao = 1;
    params[1].tipo = DOUBLE_PAR;
    params[1].amarrado = 0;
    params[1].posicao = 3;
    params[2].tipo = DOUBLE_PAR;
    params[2].amarrado = 0;
    params[2].posicao = 2;

    foo = gera_func(calcula, 3, params);
    a = 2;
    b = 142341234.32144;
    c = 3.1;
    printf("calcula(%d,%f,%f) = %f\n", a, b, c, calcula(a, b, c));
    printf("foo(%d,%f,%f) = %f\n", a, b, c, foo(a, b, c));

    return 0;
}
