#include <stdio.h>
#include "../lib/imageio.h"

// This program reads an image, print out the information about the four corners
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
	// Get the integer array that contains all the pixels
	int* data = image_pixels(i);
	// Print detailed pixel information about top-left corner
	// Remember array index starts from 0
	// so the top-left pixel is the first element
	printf("Top-left corner: 0x%X\n", data[0]);
	// Print detailed pixel information about left-top corner
	// The first row of the image are saved continuously at the beginning
	// so the rop-right pixel is the width-th element
	printf("Top-right corner: 0x%X\n", data[width - 1]);
	// Print detailed pixel information about bottom-left corner
	// To get to the bottom row, skip (height - 1) rows
	// each row contains width pixels, so using (height - 1) * width
	// as index gives us the bottom-left corner
	printf("Bottom-left corner: 0x%X\n", data[(height - 1) * width]);
	// Print detailed pixel information about bottom-right corner
	// This is the last pixel, the whole image contains width * height pixels
	// so minus 1 (recall index starts from 0) gives us the last pixel
	printf("Bottom-right corner: 0x%X\n", data[width * height - 1]);
	// Close the IMAGE object, given in the first parameter, release all
	// associated resources
	image_close(i);
	return 0;
}
