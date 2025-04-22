/************************************************************************
**
** NAME:        steganography.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**				Justin Yokota - Starter Code
**				YOUR NAME HERE
**
** DATE:        2020-08-23
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "imageloader.h"

//Determines what color the cell at the given row/col should be. This should not affect Image, and should allocate space for a new Color.
Color *evaluateOnePixel(Image *image, int row, int col)
{
	//YOUR CODE HERE
	Color *ic = malloc(sizeof(Color));
	int index = row * image->cols + col;

	if (image->image[index]->B & 1) {
		ic->B = 255;
		ic->G = 255;
		ic->R = 255;
	} else {
		ic->B = 0;
		ic->G = 0;
		ic->R = 0;
	}

	return ic;
}

//Given an image, creates a new image extracting the LSB of the B channel.
Image *steganography(Image *image)
{
	//YOUR CODE HERE

	Image *imp = malloc(sizeof(Image));
	imp->cols = image->cols;
	imp->rows = image->rows;
	int index;
	int size = image->cols * image->rows;

	Color **colorlist = malloc(size * sizeof(Color*));
	imp->image = colorlist;

	for (int i = 0; i < image->rows; i++) {
		for (int j = 0; j < image->cols; j++) {
			index = i * image->cols + j;
			colorlist[index] = evaluateOnePixel(image, i, j);
		}
	}

	return imp;
}
		

/*
Loads a file of ppm P3 format from a file, and prints to stdout (e.g. with printf) a new image, 
where each pixel is black if the LSB of the B channel is 0, 
and white if the LSB of the B channel is 1.

argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a file of ppm P3 format (not necessarily with .ppm file extension).
If the input is not correct, a malloc fails, or any other error occurs, you should exit with code -1.
Otherwise, you should return from main with code 0.
Make sure to free all memory before returning!
*/
int main(int argc, char **argv)
{
	//YOUR CODE HERE
	if (argc != 2) {
		perror("The number of arguments are incorrect !");
		return -1;
	} 
	if (!isPPM(argv[1])) {
		perror("The format of file is incorrect !");
		return -1;
	} 
	Image *ip_without_secret = readData(argv[1]);
	Image *ip_with_secret = steganography(ip_without_secret);

	writeData(ip_with_secret);

	freeImage(ip_with_secret);
	freeImage(ip_without_secret);

	return 0;
}
