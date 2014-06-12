SRC = gera_func.c
CC = gcc
CC_ARGS = -Wa,--execstack -Wall -m32 -o
OBJ_ARGS = -m32 -c
CC_DEBUG_ARGS = -D_DEBUG -g
BIN = pow boo prefixo calcula testes-prof

debug =	$(CC) $(CC_DEBUG_ARGS) $(CC_ARGS)$(2)$(1) $(SRC) -lm
compile = $(CC) $(CC_ARGS)$(2)$(1) $(SRC) -lm
dump = hexdump -C debug_func.bin

main: pow boo prefixo calcula testes-prof
	@echo "** Compiled Everything **"

pow:
	$(call debug, pow.c, pow)
	@echo "** Compiled pow **"
	@echo "** Running pow **"
	./pow
	$(dump)

boo:
	$(call debug, boo.c, boo)
	@echo "** Compiled boo **"
	@echo "** Running boo **"
	./boo
	$(dump)

prefixo:
	$(call debug, prefixo.c, prefixo)
	@echo "** Compiled prefixo **"
	@echo "** Running prefixo **"
	./prefixo
	$(dump)

calcula:
	$(call debug, calcula.c, calcula)
	@echo "** Compiled calcula **"
	@echo "** Running calcula **"
	./calcula
	$(dump)

testes-prof:
	$(call debug, testes-prof.c, testes-prof)
	@echo "** Compiled testes-prof **"
	@echo "** Running testes-prof **"
	./testes-prof
	$(dump)

prototype:
	$(CC) $(OBJ_ARGS) prototype.s
	objdump -d prototype.o

clean:
	rm -f *.o *.bin $(BIN)

.PHONY: main pow boo prefixo calcula testes-prof