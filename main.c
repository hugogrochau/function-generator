   #include <math.h>
   #include <stdio.h>
   #include "gera_func.h"

   typedef double (*func_ptr) (double x);

   int main (void) {
      Parametro params[2];
      func_ptr f_quadrado = NULL;
      double d;
      int i;

      params[0].tipo = DOUBLE_PAR;
      params[0].amarrado = 0;
      params[0].posicao = 1;
      params[1].tipo = DOUBLE_PAR;
      params[1].amarrado = 1;
      params[1].valor.v_double = 2.0;

      f_quadrado = (func_ptr) gera_func (pow, 2, params);

      for (i = 1; i <= 10; i++) {
         d = f_quadrado(i);
         printf("%d ^ 2 = %f\n", i, d);
      }

      libera_func(f_quadrado);
      return 0;
   }
