/************************************************************************
**
** NAME:        gameoflife.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Justin Yokota - Starter Code
**				YOUR NAME HERE
**
**
** DATE:        2020-08-23
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "imageloader.h"

const char *Errorinfo = 
	"Usage: ./gameOfLife filename rule\n"
	"filename is an ASCII PPM file (type P3) with maximum value 255.\n"
	"rule is a hex number beginning with 0x; Life is 0x1808.\n";

//Calculate the coordinate of neighborhood
int ring(int val, int max) 
{
	if (val < 0 ) return max - 1;
	if (val >= max) return 0;
	return val;
}

//Determines what color the cell at the given row/col should be. This function allocates space for a new Color.
//Note that you will need to read the eight neighbors of the cell in question. The grid "wraps", so we treat the top row as adjacent to the bottom row
//and the left column as adjacent to the right column.
Color *evaluateOneCell(Image *image, int row, int col, uint32_t rule)
{
	//YOUR CODE HERE
	//目的，我需要判断这个cell在下一轮中的是lived还是died
	//1.我要分图层来判断，分别是RGB这三个图层
	//2.0 我知道规则，最简单的life规则是对应的代码时0x1808，18是对应当前是lived的cell，08是对应当前是died的cell。
	//2.1 对于单个图层，判断这个点的周围八个点分别是lived还是died，然后根据lived点的数量来判断下一个周它的值。
	//2.2 对于边界使用wraps：一种边界判断。超出上边界当y=-1时，y=rows-1;超出下边界当y=rows时，y=0；超出左边界x=-1时，x=cols-1;超出右边界x=cols时，x=0;
	//2.3 计数判断8个邻居的状态，然后通过规则来确定这个cell的这个图层是lived还是died
	Color *ic = malloc(sizeof(Color));
	int index = row * image->cols + col;
	uint8_t Rcur, Gcur, Bcur;
	uint8_t numberofneighofR = 0, numberofneighofG = 0, numberofneighofB = 0;
	int8_t offsetcol[8] = {-1, 0, 1, -1, 1, -1, 0, 1};
	int8_t offsetrow[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
	int neighindex;

	Rcur = image->image[index]->R;
	Gcur = image->image[index]->G;
	Bcur = image->image[index]->B;

	for (int i = 0; i < 8; i++) {
		neighindex = ring(row + offsetrow[i], image->rows) * image->cols + ring(col + offsetcol[i], image->cols);
		//R
		if (image->image[neighindex]->R == 255) {
			numberofneighofR++;
		}
		if (image->image[neighindex]->G == 255) {
			numberofneighofG++;
		}
		if (image->image[neighindex]->B == 255) {
			numberofneighofB++;
		} 
	}

	if (Rcur == 255) {
		ic->R = 255 * ((rule >> (9 + numberofneighofR) & 1));
	} else {
		ic->R = 255 * ((rule >> numberofneighofR) & 1);
	}
	if (Gcur == 255) {
		ic->G = 255 * ((rule >> (9 + numberofneighofG) & 1));
	} else {
		ic->G = 255 * ((rule >> numberofneighofG) & 1);
	}
	if (Bcur == 255) {
		ic->B = 255 * ((rule >> (9 + numberofneighofB) & 1));
	} else {
		ic->B = 255 * ((rule >> numberofneighofB) & 1);
	}

	return ic;
}

//The main body of Life; given an image and a rule, computes one iteration of the Game of Life.
//You should be able to copy most of this from steganography.c
Image *life(Image *image, uint32_t rule)
{
	//YOUR CODE HERE
	Image * imp = (Image*)malloc(sizeof(Image));
	imp->cols = image->cols;
	imp->rows = image->rows;

	int size, index;
	size = image->cols * image->rows;

	Color **colorlist = (Color**)malloc(size * sizeof(Color*));
	imp->image = colorlist;

	for (int i = 0; i < image->rows; i++) {
		for (int j = 0; j < image->cols; j++) {
			index = i * image->cols + j;
			colorlist[index] = evaluateOneCell(image, i, j, rule);
		}
	}

	return imp;
}

/*
Loads a .ppm from a file, computes the next iteration of the game of life, then prints to stdout the new image.

argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a .ppm.
argv[2] should contain a hexadecimal number (such as 0x1808). Note that this will be a string.
You may find the function strtol useful for this conversion.
If the input is not correct, a malloc fails, or any other error occurs, you should exit with code -1.
Otherwise, you should return from main with code 0.
Make sure to free all memory before returning!

You may find it useful to copy the code from steganography.c, to start.
*/
int main(int argc, char **argv)
{
	//YOUR CODE HERE
	if (argc != 3) {
		fprintf(stderr, "%s", Errorinfo);
		return -1;
	}
	if (!isPPM(argv[1])) {
		perror("The format of file is incorrect !");
		return -1;
	} 

	Image *ip_cur = readData(argv[1]);
	Image *ip_gerenation = life(ip_cur, (uint32_t)strtoul(argv[2], NULL, 0));

	writeData(ip_gerenation);

	freeImage(ip_cur);
	freeImage(ip_gerenation);

	return 0;
}
