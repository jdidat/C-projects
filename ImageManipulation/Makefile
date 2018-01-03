CC = gcc

OPTIONS = -Werror -Wall -std=c99

LIBS = -lm `pkg-config --cflags --libs gtk+-2.0`

all: ../lib/imageio.o channels.c
	$(CC) $(OPTIONS) $(LIBS) -o channels ../lib/imageio.o channels.c

clean:
	rm -f channels *.o
