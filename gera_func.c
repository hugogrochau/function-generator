/* Hugo Pedrotti Grochau    1310486 3WA */
/* Lorenzo Saraiva          1110649 3WA */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gera_func.h"

#define NUM_BYTES_INICIALIZACAO 3
#define NUM_BYTES_FINALIZACAO 2
#define NUM_BYTES_CALL 8

#define OP_CALL 0xE8
#define OP_ADD_ESP 0xC483
#define OP_PUSH_BYTE 0x6A
#define OP_PUSH_WORD 0x68
#define OP_PUSH_EBP 0x75FF

typedef unsigned char Byte;
typedef Byte *ByteArray;
typedef unsigned int Word;
typedef unsigned short DoisBytes;

size_t calc_num_bytes_instrucao(int n, Parametro params[]);
void gera_instrucoes(int n, Parametro params[], ByteArray bytes_func, void *f);
size_t tam_tipo_param(Parametro *param);
size_t posicao_na_pilha(int n, Parametro params[], int posicao);

void *gera_func(void *f, int n, Parametro params[]) {
    const Byte bytes_inicializacao[NUM_BYTES_INICIALIZACAO] = {0x55, 0x89,
                                                               0xe5};
    const Byte bytes_finalizacao[NUM_BYTES_FINALIZACAO] = {0xc9, 0xc3};
    ByteArray bytes_nova_func;

#ifdef _DEBUG
    FILE *fp = fopen("debug_func.bin", "w");
#endif

    size_t num_bytes_params = calc_num_bytes_instrucao(n, params);
    /* alocando o espaço necessário para todas as instruções */
    void *nova_func = malloc(NUM_BYTES_INICIALIZACAO +
                             num_bytes_params +
                             NUM_BYTES_CALL +
                             NUM_BYTES_FINALIZACAO);
    /* criando um ponteiro para o começo dos bytes da nova função que será
       deslocado conforme as instruções forem inseridas */
    bytes_nova_func = (ByteArray) nova_func;

    /* inserindo as instruções de inicialização */
    memcpy(bytes_nova_func, bytes_inicializacao, NUM_BYTES_INICIALIZACAO);
    bytes_nova_func += NUM_BYTES_INICIALIZACAO;

    /* inserindo as instruções de push, call e liberação da pilha */
    gera_instrucoes(n, params, bytes_nova_func, f);
    bytes_nova_func += num_bytes_params;
    bytes_nova_func += NUM_BYTES_CALL;

    /* inserindo as instruções de finalização */
    memcpy(bytes_nova_func, bytes_finalizacao, NUM_BYTES_FINALIZACAO);

#ifdef _DEBUG
    fwrite(nova_func, sizeof(Byte),
           NUM_BYTES_INICIALIZACAO +
           num_bytes_params +
           NUM_BYTES_CALL +
           NUM_BYTES_FINALIZACAO,
           fp);
    fclose(fp);
#endif

    return nova_func;
}

void libera_func(void *f) {
    if (f != NULL)
        free(f);
}

/* Calcula o número de bytes de instruções para dar push em todos os
   parâmetros */
size_t calc_num_bytes_instrucao(int n, Parametro params[]) {
    size_t num_bytes = 0;
    while (n--) {
        switch (params[n].tipo) {
        case DOUBLE_PAR:
            if (params[n].amarrado)
                num_bytes += 10; /* push CONST x2 */
            else
                num_bytes += 6; /* push offset(%ebp) x2 */
            break;
        case INT_PAR:
        case PTR_PAR:
            if (params[n].amarrado)
                num_bytes += 5; /* push COST */
            else
                num_bytes += 3; /* push offset(%ebp) */
            break;
        case CHAR_PAR:
            if (params[n].amarrado)
                num_bytes += 2; /* pushb CONST */
            else
                num_bytes += 3; /* push offset(%ebp) */
            break;
        }
    }
    return num_bytes;
}

/* Gera e insere as instruções necessárias para executar a chamada da função
   original */
void gera_instrucoes(int n, Parametro params[], ByteArray bytes_func, void *f) {
    int i = n, posicao;
    size_t num_bytes = 0;
    /* "pushando" os parâmetros da direita para a esquerda */
    while (i--) {
        if (params[i].amarrado) {
            /* push params[i].valor */
            switch (params[i].tipo) {
            case DOUBLE_PAR:
                /* "pushando" o double de 4 a 4 bytes */
                *(bytes_func++) = OP_PUSH_WORD;
                *((Word *)bytes_func) = ((Word *) &params[i].valor.v_double)[1];
                bytes_func += sizeof(Word);
                *(bytes_func++) = OP_PUSH_WORD;
                *((Word *)bytes_func) = ((Word *) &params[i].valor.v_double)[0];
                bytes_func += sizeof(Word);
                break;
            case INT_PAR:
            case PTR_PAR:
                *(bytes_func++) = OP_PUSH_WORD;
                /* na memória não importa se acessamos a union como uma v_int
                   ou v_ptr */
                *((Word *)bytes_func) = (Word) params[i].valor.v_int;
                bytes_func += sizeof(Word);
                break;
            case CHAR_PAR:
                *(bytes_func++) = (Byte) OP_PUSH_BYTE;
                *(bytes_func++) = (char) params[i].valor.v_char;
            }
        } else {
            posicao = posicao_na_pilha(n, params, params[i].posicao);
            /* push posicao(%ebp) */
            switch (params[i].tipo) {
            case DOUBLE_PAR:
                *((DoisBytes *) bytes_func) = OP_PUSH_EBP;
                bytes_func += sizeof(DoisBytes);
                *(bytes_func++) = (Byte) posicao + sizeof(Word);
            case INT_PAR:
            case PTR_PAR:
            case CHAR_PAR:
                *((DoisBytes *) bytes_func) = OP_PUSH_EBP;
                bytes_func += sizeof(DoisBytes);
                *(bytes_func++) = (Byte) posicao;
                break;
            }
        }
        num_bytes += tam_tipo_param(&params[i]);
    }
    /* call f */
    *(bytes_func++) = OP_CALL;
    /* calculando o offset */
    *((void **) bytes_func) = (void *) (int) f -
                              ((int) bytes_func + 4);
    bytes_func += sizeof(void *);

    /* add num_bytes, %esp */
    *((DoisBytes *) bytes_func) = OP_ADD_ESP;
    bytes_func += sizeof(DoisBytes);
    *(bytes_func++) = (Byte) num_bytes;
}

/* Retorna o tamanho do tipo de dado na pilha de execução */
size_t tam_tipo_param(Parametro *param) {
    if (param->tipo == DOUBLE_PAR)
        return sizeof(double);
    return sizeof(Word);
}

/* Retorna a posição do parâmetro na pilha */
size_t posicao_na_pilha(int n, Parametro params[], int posicao) {
    int i;
    size_t num_bytes = 0;
    for (i = 0; i < n; i++)
        /* Se o parâmetro não for amarrado e vier antes */
        if (!params[i].amarrado && params[i].posicao < posicao)
            num_bytes += tam_tipo_param(&params[i]);
    return num_bytes + 8;
}
