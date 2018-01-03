// do not modify the line after the line after next
// do not modify the line after next
// do not modify the next line
#include "../lib/imageio.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[])
{
  // TODO
  // Step 1: Load original image
  IMAGE i = image_open(argv[1]);
  int h = image_height(i);
  int w = image_width(i);
  int area = h * w;
  IMAGE r = image_clone(i);
  IMAGE g = image_clone(i);
  IMAGE b = image_clone(i);
  // Step 2: Separate channels
  int * rArray = image_pixels(r);
  int * gArray = image_pixels(g);
  int * bArray = image_pixels(b);
  int j;
  unsigned int x = 0xFFFF0000;
  for(j = 0; j < area; j++) {
    bArray[j] = bArray[j] & x;
  }
  int k;
  unsigned int y = 0xFF00FF00;
  for (k = 0; k < area; k++) {
    gArray[k] = gArray[k] & y;
  }
  int l;
  unsigned int z = 0xFF0000FF;
  for (l = 0; l < area; l++) {
    rArray[l] = rArray[l] & z;
  }
  // Step 3: Save each channels to three files
  char * d;
  d = strtok(argv[1], "..//.");
  d = strtok(NULL, "..//.");
  char e[25];
  strcpy(e, d);
  char f[25];
  strcpy(f, d);
  strcat(d, "_red.jpg");
  printf("%s\n", d);
  strcat(e, "_blue.bmp");
  printf("%s\n", e);
  strcat(f, "_green.tif");
  printf("%s\n", f);
  if (image_save(r, d)) {
    return -1;
  }
  if (image_save(b, e)) {
    return -1;
  }
  if (image_save(g, f)) {
    return -1;
  }
  // Step 4: Finalization
  image_close(i);
  image_close(r);
  image_close(g);
  image_close(b);
  return 0;
}
