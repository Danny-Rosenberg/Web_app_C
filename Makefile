CC = clang
ARGS = -Wall

all: httpserver

httpserver: httpserver_play.c
	$(CC) -lpthread -o httpserver $(ARGS) httpserver_play.c -g


clean:
	rm -rf httpserver *.o

