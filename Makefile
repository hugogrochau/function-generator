SRC = main.c gera_func.c
CC = gcc
CC_ARGS = -Wa,--execstack -Wall -ansi -lm -o
CC_DEBUG_ARGS = -g
BIN = exec

main: 
	$(CC) $(CC_DEBUG_ARGS) $(CC_ARGS) $(BIN) $(SRC) 

run: main
	./$(BIN) 

clean:
	rm $(BIN)
