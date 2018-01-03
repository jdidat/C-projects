#include "../lib/imageio.h"

// This program reads an image, save multiple copies into different formats
int main()
{
	// Call image_open to access an image. 
	// The parameter is the (relative) path and file name to the image you
	// want to open. Returns an IMAGE object.
	IMAGE i = image_open("../images/Lenna.png");
	// check whether open succeeded
	if (i == NULL)
		return -1;
	// Without doing any changes, save the IMAGE object given in the first
	// parameter to the file given in the second parameter.
	if (image_save(i, "test.png"))
		return -1;
	// Save the same IMAGE object to another format, which is specified
	// in the file extension
	if (image_save(i, "test.jpg"))
		return -1;
	// Save the same IMAGE object to BMP format
	if (image_save(i, "test.bmp"))
		return -1;
	// Save the same IMAGE object to TIFF format
	if (image_save(i, "test.tiff"))
		return -1;
	// Close the IMAGE object, given in the first parameter, release all
	// associated resources
	image_close(i);
	printf("File saved! Check the current folder for output!\n");
	return 0;
}
