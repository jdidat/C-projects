CC = gcc

OPTIONS = -Werror -Wall -std=c99

LIBS = -lm `pkg-config --cflags --libs gtk+-2.0`

all: ../lib/imageio.o negate.c
	$(CC) $(OPTIONS) $(LIBS) -o negate ../lib/imageio.o negate.c

clean:
	rm -f negate *.o
