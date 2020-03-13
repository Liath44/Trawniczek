#include <stdlib.h>
#include "picasso8.h"

void InitializeFileHeader(fileheader *fh, uint32_t hsize, uint32_t palsize, uint32_t width, uint32_t height)
	{
	fh -> signature[0] = 'B';
	fh -> signature[1] = 'M';
	fh -> filesize = width * height * BPM / 8 + hsize + palsize;
	fh -> reserved = 0;
	fh -> offset = hsize + palsize;
	}

void InitializeBitmapInfoHeader(bitmapinfoheader *bih, uint32_t width, uint32_t height, uint32_t nocolours)
	{
	bih -> headersize = HSIZE;
	bih -> width = width;
	bih -> height = height;
	bih -> planes = 1;
	bih -> bitsperpixel = BPM;
	bih -> compression = 0;
	bih -> imagesize = 0;
	bih -> ypixelpermeter = 0;
	bih -> xpixelpermeter = 0;
	bih -> numcolorspallette = nocolours;
	bih -> mostimpcolor = 0;
	}
/*
 * Writes palette to bitmap
 *
 * int nocolours - number of colours in palette
 */
void WritePalette(FILE *bitmap, FILE *palette, int nocolours)
	{
	uint8_t piv[4];	//stores single colour
	int tocast[3];	//for storing taken arguments
	piv[3] = 0;
	for(int i = 0; i < nocolours; i++)
			{
			fscanf(palette, "%d", tocast);
			fscanf(palette, "%d", tocast + 1);
			fscanf(palette, "%d", tocast + 2);
			piv[0] = (uint8_t)tocast[0];
			piv[1] = (uint8_t)tocast[1];
			piv[2] = (uint8_t)tocast[2];
			fwrite(piv, sizeof(*piv), 4, bitmap);//write to bitmap
			}
	}

/*
 * Frees memory required for initializing bitmap
 */
void FreeMem(headers *h, FILE *palette)
	{
	free(h);
	fclose(palette);
	}

int InitializeBitmap(FILE *bitmap, uint32_t width, uint32_t height)
	{
	headers *h = malloc(sizeof *h);
	FILE *palette = fopen(PNAME, "r");
	if(h == NULL || palette == NULL)
		return 0;
	uint32_t nocolours;
	fscanf(palette, "%u", &nocolours);
	InitializeFileHeader(&(h -> header1), sizeof *h, nocolours * sizeof(uint32_t), width, height);
	InitializeBitmapInfoHeader(&(h -> header2), width, height, nocolours);
	fwrite(h, 1, sizeof *h, bitmap);
	WritePalette(bitmap, palette, nocolours);
	FreeMem(h, palette);
	return 1;
	}

void PutPixel(FILE *bitmap, uint8_t pixel)
	{
	fprintf(bitmap, "%c", pixel);
	}

void PixelLawn(FILE *bitmap, char mode, uint8_t saturation)
	{
	if(mode == 'x')
		{
		PutPixel(bitmap, 0);
		}
	else if(mode == 'n')
		{
		PutPixel(bitmap, 1);
		}
	else if(mode == 'w')
		{
		if(1 <= saturation && saturation <= 12)
			PutPixel(bitmap, 1 + saturation);
		else
			PutPixel(bitmap, 30);
		}
	else if(mode == 'o')
		{
		if(1 <= saturation && saturation <= 16)
			PutPixel(bitmap, saturation + 13);
		else
			PutPixel(bitmap, 30);
		}
	else
		{
		PutPixel(bitmap, 30);
		}
	}
