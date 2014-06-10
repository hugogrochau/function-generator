#include <stdio.h>
#include <string.h>
#include <float.h>
#include "gera_func.h"

double calcula(int a, double b, double c) {
    return a + b + c;
}

typedef double (*func_ptr) (int a, double b);

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
    params[1].posicao = 2;
    params[2].tipo = DOUBLE_PAR;
    params[2].amarrado = 1;
    params[2].valor.v_double = DBL_MAX;


    foo = gera_func(calcula, 3, params);
    a = 2;
    b = DBL_MIN;
    c = DBL_MAX;
    printf("%llx\n", b);
    printf("%llx\n", c);

    printf("calcula(%d,%lf,%lf) = %lf\n", a, b, c, calcula(a, b, c));
    printf("foo(%d,%lf) = %lf\n", a, b, foo(a, b));

    return 0;
}
