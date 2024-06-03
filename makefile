CC = gcc
.PHONY: all clean

all: server client

server: server.c
	$(CC) -o server server.c -lncurses -lform

client: client.c
	$(CC) -o client client.c -lncurses -lform

clean:
	rm -f server client