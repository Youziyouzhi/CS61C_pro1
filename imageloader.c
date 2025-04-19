/************************************************************************
**
** NAME:        imageloader.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**              Justin Yokota - Starter Code
**				YOUR NAME HERE
**
**
** DATE:        2020-08-15
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include "imageloader.h"

//Opens a .ppm P3 image file, and constructs an Image object. 
//You may find the function fscanf useful.
//Make sure that you close the file with fclose before returning.
Image *readData(char *filename) 
{
	//YOUR CODE HERE
	FILE *fp = fopen(filename,"r");

	char fileformat[5];
	uint32_t _cols;
	uint32_t _rows;
	uint32_t maxvalue;
	Image *ip = malloc(sizeof(Image));

	fscanf(fp, "%s %d %d %d", fileformat, &_cols, &_rows, &maxvalue);

	ip->cols = _cols;
	ip->rows = _rows;
	
	Color **colorlist = malloc(_rows * _cols * sizeof(Color*));
	ip->image = colorlist;

	uint8_t red, green, blue;

	for (int i = 0; i < _cols * _rows; i++) {
		colorlist[i] = malloc(sizeof(Color));
		fscanf(fp, "%hhu %hhu %hhu", &red, &green, &blue);
		colorlist[i]->R = red;
		colorlist[i]->G = green;
		colorlist[i]->B = blue;
	}

	fclose(fp);

	return ip;
}

//Given an image, prints to stdout (e.g. with printf) a .ppm P3 file with the image's data.
void writeData(Image *image)
{
	//YOUR CODE HERE
	printf("P3\n");
	printf("%d %d\n", image->cols, image->rows);
	printf("255\n");
	uint32_t index;
	for (int i = 0; i < image->rows; i++) {
		for (int j = 0; j < image->cols; j++) {
			index = i * image->cols + j;
			printf("%3d %3d %3d", image->image[index]->R, image->image[index]->G, image->image[index]->B);
			if (j == image->cols - 1) {
				printf("\n");
			} else {
				printf("   ");
			}
		}
	}
}

//Frees an image
void freeImage(Image *image)
{
	//YOUR CODE HERE
	for (int i = 0; i < image->cols * image->rows; i++) {
		free(image->image[i]);
	}
	free(image->image);
	free(image);
}