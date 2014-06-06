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

size_t calc_num_bytes_params(int n, Parametro params[]);
void gera_instrucoes(int n, Parametro params[], ByteArray bytes_func, void *f);
void *calc_offset(void *f, ByteArray addr);
size_t tam_tipo_param(Parametro *param);
size_t posicao_na_pilha(int n, Parametro params[], int posicao);

void *gera_func(void *f, int n, Parametro params[]) {
    Byte bytes_inicializacao[NUM_BYTES_INICIALIZACAO] = {0x55, 0x89, 0xe5};
    Byte bytes_finalizacao[NUM_BYTES_FINALIZACAO] = {0xc9, 0xc3};
    ByteArray bytes_nova_func;

#ifdef _DEBUG
    FILE *fp = fopen("debug_func.bin", "w");
#endif

    size_t num_bytes_params = calc_num_bytes_params(n, params);

    void *nova_func = malloc(NUM_BYTES_INICIALIZACAO +
                             num_bytes_params +
                             NUM_BYTES_CALL +
                             NUM_BYTES_FINALIZACAO);
    bytes_nova_func = (ByteArray) nova_func;

    memcpy(bytes_nova_func, bytes_inicializacao, NUM_BYTES_INICIALIZACAO);
    bytes_nova_func += NUM_BYTES_INICIALIZACAO;

    gera_instrucoes(n, params, bytes_nova_func, f);
    bytes_nova_func += num_bytes_params;
    bytes_nova_func += NUM_BYTES_CALL;

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
    free(f);
}

size_t calc_num_bytes_params(int n, Parametro params[]) {
    int i;
    size_t num_bytes = 0;
    for (i = 0; i < n; i++) {
        switch (params[i].tipo) {
        case DOUBLE_PAR:
            if (params[i].amarrado)
                num_bytes += 10;
            else
                num_bytes += 6;
            break;
        case INT_PAR:
        case PTR_PAR:
            if (params[i].amarrado)
                num_bytes += 5;
            else
                num_bytes += 3;
            break;
        case CHAR_PAR:
            if (params[i].amarrado)
                num_bytes += 2;
            else
                num_bytes += 3;
            break;
        }
    }
    return num_bytes;
}

void gera_instrucoes(int n, Parametro params[], ByteArray bytes_func, void *f) {
    int i, posicao;
    size_t num_bytes = 0;
    for (i = 0; i < n; i++) {
        if (params[i].amarrado) {
            switch (params[i].tipo) {
            case DOUBLE_PAR:
                *(bytes_func++) = OP_PUSH_WORD;
                *((Word *)bytes_func) = (Word) params[i].valor.v_double >> sizeof(Word);
                bytes_func += sizeof(Word);
                *(bytes_func++) = OP_PUSH_WORD;
                *((Word *)bytes_func) = (Word) params[i].valor.v_double;
                bytes_func += sizeof(Word);
                break;
            case INT_PAR:
            case PTR_PAR:
                *(bytes_func++) = OP_PUSH_WORD;
                *((Word *)bytes_func) = (Word) params[i].valor.v_int;
                bytes_func += sizeof(Word);
                break;
            case CHAR_PAR:
                *(bytes_func++) = (Byte) OP_PUSH_BYTE;
                *(bytes_func++) = (char) params[i].valor.v_char;
            }
        } else {
            posicao = posicao_na_pilha(n, params, params[i].posicao);
            switch (params[i].tipo) {
            case DOUBLE_PAR:
                *((DoisBytes *) bytes_func) = OP_PUSH_EBP;
                bytes_func += sizeof(DoisBytes);
                *(bytes_func++) = (Byte) posicao + 4;
                *((DoisBytes *) bytes_func) = OP_PUSH_EBP;
                bytes_func += sizeof(DoisBytes);
                *(bytes_func++) = (Byte) posicao;
                break;
            case INT_PAR:
            case PTR_PAR:
            case CHAR_PAR:
                *((DoisBytes *) bytes_func) = OP_PUSH_EBP;
                bytes_func += sizeof(DoisBytes);
                *(bytes_func++) = (Byte) posicao;
                break;
            }
            num_bytes += tam_tipo_param(&params[i]);
        }
    }
    *(bytes_func++) = OP_CALL;
    *((void **) bytes_func) = (void *) (unsigned int) f - ((unsigned int) bytes_func + 3);
    bytes_func += sizeof(void *);

    *((DoisBytes *) bytes_func) = OP_ADD_ESP;
    bytes_func += sizeof(DoisBytes);
    *(bytes_func++) = (Byte) num_bytes;
}

size_t tam_tipo_param(Parametro *param) {
    if (param->tipo == DOUBLE_PAR)
        return sizeof(double);
    return sizeof(Word);
}

size_t posicao_na_pilha(int n, Parametro params[], int posicao) {
    int i;
    size_t tam_bytes = 0;
    for (i = 0; i < n; i++)
        if (!params[i].amarrado && params[i].posicao < posicao)
            tam_bytes += tam_tipo_param(&params[i]);
    return tam_bytes + 8;
}
