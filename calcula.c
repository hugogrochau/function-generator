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

    printf("calcula(2, DBL_MIN, DBL_MAX) = %lf\n",
            calcula(2, DBL_MIN, DBL_MAX));
    printf("foo(2, DBL_MIN) = %lf\n",
            foo(2, DBL_MIN));

    return 0;
}
