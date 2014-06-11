SRC = gera_func.c
CC = gcc
CC_ARGS = -Wa,--execstack -Wall -m32 -ansi -o
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

boo:
	$(call debug, boo.c, boo)
	@echo "** Compiled boo **"

prefixo:
	$(call debug, prefixo.c, prefixo)
	@echo "** Compiled prefixo **"

calcula:
	$(call debug, calcula.c, calcula)
	@echo "** Compiled calcula **"

testes-prof:
	$(call debug, testes-prof.c, testes-prof)
	@echo "** Compiled testes-prof **"

run: main
	@echo "** Running pow **"
	./pow
	$(dump)
	@echo "** Running boo **"
	./boo
	$(dump)
	@echo "** Running prefixo **"
	./prefixo
	$(dump)
	@echo "** Running calcula **"
	./calcula
	$(dump)
	@echo "** Running testes-prof **"
	./testes-prof
	$(dump)
	rm debug_func.bin

prototype:
	$(CC) $(OBJ_ARGS) prototype.s
	objdump -d prototype.o

clean:
	rm -f *.o *.bin $(BIN)

.PHONY: main pow boo prefixo calcula testes-prof