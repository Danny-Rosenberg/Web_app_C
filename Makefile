CC = clang
ARGS = -Wall

all: httpserver

httpserver: httpserver.c
	$(CC) -lpthread -o httpserver $(ARGS) httpserver.c -g


clean:
	rm -rf httpserver *.o

