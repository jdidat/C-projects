CC = gcc

OPTIONS = -g -Werror -Wall -std=c99

LIBS = -lm `pkg-config --cflags --libs gtk+-2.0`

all: ../lib/imageio.o intensity.c
	$(CC) $(OPTIONS) $(LIBS) -o intensity ../lib/imageio.o intensity.c

clean:
	rm -f intensity *.o *.jpg *.png *.out *.txt
