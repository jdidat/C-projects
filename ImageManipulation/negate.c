#include <stdio.h>
#include <string.h>
#include "../lib/imageio.h"

int main(int argc, char* argv[])
{
	// TODO
	// Step 1: Load original image
  IMAGE i = image_open(argv[1]);
  int h = image_height(i);
  int w = image_width(i);
  int area = h * w;
  IMAGE n = image_clone(i);
	// Step 2: Negate it
  int * nArray = image_pixels(n);
  int x = 0x00FFFFFF;
  int j;
  for (j = 0; j < area; j++) {
    nArray[j] = nArray[j] ^ x;
  }
  char * d;
  d = strtok(argv[1], "..//.");
  d = strtok(NULL, "..//.");
  strcat(d, "_negation.bmp");
  printf("%s\n", d);
  if (image_save(n, d)) {
    return -1;
  }
	// Step 4: Finalization
  image_close(i);
  image_close(n);
  return 0;
}
