#include <stdio.h>
#include "../lib/imageio.h"

// global const parameter
const int width = 320;
const int height = 240;

// This program create an empty image, draw a letter 'H' (short for "Hello world!") on it
int main()
{
	// Call image_new to create an image.
	// Parameters are width an height respectively
	IMAGE i = image_new(width, height);
	// check whether image creation succeeded
	if (i == NULL)
		return -1;
	// Fill the image with white background
	image_fill(i, 0xFFFFFFFF);
	// Get the integer array that contains all the pixels
	int* data = image_pixels(i);
	// Recall top-left corner is the original
	// from left to right is +x direction
	// from top to bottom is +y direction
	int x, y;
	// The vertical stroke on the left
	// Starts from (100, 60) to (100, 180)
	for (y = 60; y < 180; ++y)
		data[100 + y * width] = 0xFF000000;
	// The horizontal stroke
	// From (100, 120) to (220, 120)
	for (x = 100; x < 220; ++x)
		data[x + 120 * width] = 0xFF000000;
	// The vertical stroke on the left
	// Starts from (220, 60) to (220, 180)
	for (y = 60; y < 180; ++y)
		data[220 + y * width] = 0xFF000000;
	// Save the original image, to show we didn't modify its content
	if (image_save(i, "Hello.bmp"))
		return -1;
	// Close the IMAGE object to release associated resources
	image_close(i);
	printf("File saved! Check the current folder for output!\n");
	return 0;
}
