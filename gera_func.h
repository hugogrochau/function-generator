#ifndef GERA_FUNC_H
#define GERA_FUNC_H

typedef enum {INT_PAR, CHAR_PAR, DOUBLE_PAR, PTR_PAR} TipoParam;

typedef struct {
   TipoParam tipo;  /* indica o tipo do parametro */
   int amarrado; /* indica se o parametro deve ter um valor constante */
   int posicao; /* indica um número entre <code>1</code>  e <code>n</cod> */
   union {
      int v_int;
      char v_char;
      double v_double;
      void* v_ptr;
   } valor;         /* define o valor do parametro se este for constante */
} Parametro;

void* gera_func (void* f, int n, Parametro params[]);
void libera_func (void* func);

#endif

