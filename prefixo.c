#include <stdio.h>
#include <string.h>
#include "gera_func.h"

typedef int (*func_ptr) (void *candidata, size_t n);

char a[] = "quero saber se a outra string é um prefixo dessa aqu";

int main(void) {
    Parametro params[3];
    func_ptr e_prefixo = NULL;
    char um[] = "quero saber se por acaso falei alguma bobagem";
    int tam;

    params[0].tipo = PTR_PAR;
    params[0].amarrado = 0;
    params[0].posicao = 1;
    params[1].tipo = PTR_PAR;
    params[1].amarrado = 1;
    params[1].valor.v_ptr = a;
    params[2].tipo = INT_PAR;
    params[2].amarrado = 0;
    params[2].posicao = 2;

    e_prefixo = (func_ptr) gera_func(memcmp, 3, params);

    tam = 3;
    printf("'%s' e' prefixo-%d de '%s'? %s\n", um, tam, a, e_prefixo(um, tam) ? "NAO" : "SIM");
    tam = 15;
    printf("%s e' prefixo-%d de '%s'? %s\n", um, tam, a, e_prefixo(um, tam) ? "NAO" : "SIM");
    tam = 25;
    printf("'%s' e' prefixo-%d de '%s'? %s\n", um, tam, a, e_prefixo(um, tam) ? "NAO" : "SIM");

    return 0;
}
