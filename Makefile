CC = gcc
CFLAGS = -Wall -Wextra -g -Iincludes
SRC_DIR = src
OBJ_DIR = obj
INCLUDE_DIR = includes
BIN = server

SRCS = main.c \
       $(SRC_DIR)/server.c \
       $(SRC_DIR)/request.c \
       $(SRC_DIR)/response.c \
       $(SRC_DIR)/cgi.c \
       $(SRC_DIR)/files.c \
       $(SRC_DIR)/errors.c

OBJS = $(patsubst %.c, $(OBJ_DIR)/%.o, $(notdir $(SRCS)))

all: $(BIN)

$(BIN): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(INCLUDE_DIR)/%.h
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/main.o: main.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(BIN)

.PHONY: all clean
