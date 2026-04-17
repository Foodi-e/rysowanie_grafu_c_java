CC = gcc
CFLAGS = -Wall -Wextra -std=c11
LDFLAGS = -lm

SRCS = src/main.c src/fruchterman/fruchterman.c src/tutte/tutte.c src/shared/data_loader/data_loader.c src/shared/writer/writer.c

all:
	$(CC) $(CFLAGS) $(SRCS) -o graph_layout $(LDFLAGS)

clean:
	rm -f graph_layout run_tests coords.txt