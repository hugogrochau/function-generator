SRC = main.c gera_func.c
CC = gcc
CC_ARGS = -Wa,--execstack -Wall -m32 -ansi -o
OBJ_ARGS = -m32 -c
CC_DEBUG_ARGS = -D_DEBUG -g
BIN = exec

main:
	$(CC) $(CC_ARGS) $(BIN) $(SRC) -lm

debug:
	$(CC) $(CC_DEBUG_ARGS) $(CC_ARGS) $(BIN) $(SRC) -lm

prototype:
	$(CC) $(OBJ_ARGS) prototype.s
	objdump -d prototype.o

run: main
	./$(BIN)

drun: debug
	./$(BIN)

dump:
	hexdump -C debug_func.bin

debugger: debug
	gdb ./$(BIN)

clean:
	rm *.o
	rm *.bin
	rm $(BIN)
