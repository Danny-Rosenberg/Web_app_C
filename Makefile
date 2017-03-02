CC = clang
ARGS = -Wall

all: httpserver

httpserver: httpserver.c
	$(CC) -lpthread -o httpserver $(ARGS) httpserver.c -lm -g


clean:
	rm -rf httpserver *.o

