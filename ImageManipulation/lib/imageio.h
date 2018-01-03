#include <gdk-pixbuf/gdk-pixbuf.h>

typedef GdkPixbuf* IMAGE;

// open and read from a file
// returns an IMAGE
IMAGE image_open(char* filename);

// create an empty new image
// no guarantee about the initial value (random values)
IMAGE image_new(int width, int height);

// fill the whole image with one color
int image_fill(IMAGE image, int c);

// create a copy of the original IMAGE
IMAGE image_clone(IMAGE image);

// return the width of the IMAGE
int image_width(IMAGE image);

// return the height of the IMAGE
int image_height(IMAGE image);

// return the data (int array) of the IMAGE
int* image_pixels(IMAGE image);

// save the IMAGE to a file
int image_save(IMAGE image, char* filename);

// close the IMAGE
int image_close(IMAGE image);
