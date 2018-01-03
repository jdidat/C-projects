#include <stdio.h>
#include "../lib/imageio.h"

// This program reads an image, adds a white border (of 5 pixels wide) around it
int main()
{
	// Call image_open to access an image. 
	// The parameter is the (relative) path and file name to the image you
	// want to open. Returns an IMAGE object.
	IMAGE i = image_open("../images/Lenna.png");
	// check whether open succeeded
	if (i == NULL)
		return -1;
	// make a copy of the original image file
	// so that any modification to this copy won't affect the original image
	IMAGE i_copy = image_clone(i);
	// check whether clone succeeded
	if (i_copy == NULL)
		return -1;
	// Query for the size information, pass IMAGE object as parameter
	// Returns width/height as an interger
	int width = image_width(i_copy);
	int height = image_height(i_copy);
	// Get the integer array that contains all the pixels
	int* data = image_pixels(i_copy);
	int x, y;
	// Set the vertical borders
	for (y = 0; y < height; ++y)
	{
		// Fill in the left border
		for (x = 0; x < 5; ++x)
			data[x + y * width] = 0xFFFFFFFF;
		// Fill in the right border
		for (x = width - 5; x < width; ++x)
			data[x + y * width] = 0xFFFFFFFF;
	}
	// Set the horizontal borders
	for (x = 0; x < width; ++x)
	{
		// Fill in the top border
		for (y = 0; y < 5; ++y)
			data[x + y * width] = 0xFFFFFFFF;
		// Fill in the bottom border
		for (y = height - 5; y < width; ++y)
			data[x + y * width] = 0xFFFFFFFF;
	}
	// Save the original image, to show we didn't modify its content
	if (image_save(i, "original.bmp"))
		return -1;
	// Save the modified image, to show we actually added a border
	if (image_save(i_copy, "border.png"))
		return -1;
	// Close all the used IMAGE object to release associated resources
	image_close(i);
	image_close(i_copy);
	printf("File saved! Check the current folder for output!\n");
	return 0;
}
