#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int isBigEndian(char * p) {
  float i = (float) p;
  //int * i = (int *) p;
  printf("%d\n", i); 
  //printf("%c%c%c%c%c%c%c%c%c%c\n", *p, *(p + 1), *(p + 2), *(p + 3), *(p + 4), *(p + 5), *(p + 6), *(p + 7), *(p + 8), *(p + 9));
  //printf("%c%c%c%c%c%c%c%c%c%c\n", *i, *(i + 1), *(i + 2), *(i + 3), *(i + 4), *(i + 5), *(i + 6), *(i + 7), *(i + 8), *(i + 9));
  if (i == 3.1415926) {
    return 1;
  }
  return 0;
}

int main(int argc, char* argv[])
{
	if (argc < 2){
		printf("Usage: endian input\n");
		return 1;
	}
	if (isBigEndian(argv[1])){
		printf("It's Big Endian\n");
	}
	else {
		printf("It's Little Endian\n");
	}

	return 0;
}
