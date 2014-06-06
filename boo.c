#include <stdio.h>
#include <string.h>
#include "gera_func.h"

int boo(int a, int b) {
    return a - b;
}

typedef int (*func_ptr) (int x, int y);

int main(void) {
    Parametro params[2];
    func_ptr foo = NULL;
    int a, b;

    params[0].tipo = INT_PAR;
    params[0].amarrado = 0;
    params[0].posicao = 2;
    params[1].tipo = INT_PAR;
    params[1].amarrado = 0;
    params[1].posicao = 1;

    foo = gera_func(boo, 2, params);
    a = 2;
    b = 3;
    printf("boo(%d,%d) = %d\n", a, b, boo(a, b));
    printf("foo(%d,%d) = %d\n", a, b, foo(a, b));

    return 0;
}
