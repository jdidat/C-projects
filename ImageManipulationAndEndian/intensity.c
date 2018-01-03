#include "../lib/imageio.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc,char * argv[]) {
  float in = atof(argv[2]);
  IMAGE im = image_open(argv[1]);
  int h = image_height(im);
  int w = image_width(im);
  int area = h * w;
  IMAGE ic = image_clone(im);
  int * pix = image_pixels(ic);
  unsigned char * temp = (unsigned char *) pix;
  int i;
  int j = 0;
  for (i = 0; i < area; i++) {
    *(temp + j) = *(temp + j) * in;
    j++;
    *(temp + j) = *(temp + j) * in;
    j++;
    *(temp + j) = *(temp + j) * in;
    j++;
    j++;
  }
  pix = (int *) temp;
  char * d;
  d = strtok(argv[1], "..//.");
  d = strtok(NULL, "..//.");
  strcat(d, "_int.jpg");
  d[strlen(d)] = '\0';
  if (image_save(ic, d)) {
    return -1;
  }
  image_close(im);
  image_close(ic);
  return 0;
}
