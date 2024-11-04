CC = gcc
CFLAGS = -Wall -Werror
SRC = src/socket.c 
OBJ = $(SRC:src/%.c=bin/%.o)
BIN_DIR = bin

all: $(BIN_DIR) server

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

server: $(BIN_DIR)/main.o $(OBJ)
	$(CC) $(CFLAGS) -o server $(BIN_DIR)/main.o $(OBJ)

$(BIN_DIR)/main.o: main.c | $(BIN_DIR)
	$(CC) $(CFLAGS) -c main.c -o $(BIN_DIR)/main.o

bin/%.o: src/%.c | $(BIN_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BIN_DIR) server
