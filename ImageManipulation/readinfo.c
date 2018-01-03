#include <stdio.h>
#include "../lib/imageio.h"

// This program reads an image, print out the size of the image
int main()
{
	// Call image_open to access an image. 
	// The parameter is the (relative) path and file name to the image you
	// want to open. Returns an IMAGE object.
	IMAGE i = image_open("../images/Lenna.png");
	// check whether open succeeded
	if (i == NULL)
		return -1;
	// Query for the size information, pass IMAGE object as parameter
	// Returns width/height as an interger
	int width = image_width(i);
	int height = image_height(i);
	// Print out those information
	printf("size: %dx%d\n", width, height);
	// Close the IMAGE object, given in the first parameter, release all
	// associated resources
	image_close(i);
	return 0;
}
