/*
 ============================================================================
 Name        : Picasso8Test.c
 Author      : liath
 Version     : 1.0F
 Copyright   : NULL
 Description : Testing 8-bit bitmap library picasso8.h
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include "picasso8.h"

int main(void)
	{
	FILE *b = fopen("test1.bmp", "wb");
	uint32_t w1 = 400;
	uint32_t h1 = 400;
	InitializeBitmap(b, w1, h1);
	for(int i = 0; i < w1 * h1; i++)	//fill bitmap with single colour
		{
		PixelLawn(b, 'o', 1);
		}
	fclose(b);
	return EXIT_SUCCESS;
	}
