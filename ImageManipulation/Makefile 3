CC = gcc

OPTIONS = -Werror

LIBS = -lm `pkg-config --cflags --libs gtk+-2.0`

all: simplest readinfo readcorners addborder canvas

%: ../lib/imageio.o %.c
	$(CC) $(OPTIONS) $(LIBS) -o $* ../lib/imageio.o $*.c

clean:
	rm -f *.o
	rm -f simplest readinfo readcorners addborder canvas
