CC = gcc
CFLAGS = -Wall -Werror
SRC = src/socket.c src/errors.c
OBJ = $(SRC:.c=.o)

all: server

server: main.o $(OBJ)
	$(CC) $(CFLAGS) -o server main.o $(OBJ)

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

clean:
	rm -f *.o src/*.o server
