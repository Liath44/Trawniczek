#ifndef P8
#define P8

#include <stdint.h>
#include <stdio.h>

#define BPM 8 				//8bit
#define HSIZE 40 			//size of headers
#define PNAME "fpalette"	//name of palette file

#pragma pack(push,1)

/*
 * Holds bitmap's file header
 */
struct _fileheader
	{
	uint8_t signature[2];	//"BM"
	uint32_t filesize;		//width * height * BPM / 8 + headerssize + palettesize
	uint32_t reserved;		//0
	uint32_t offset;		//address of beggining of pixel array
	};
typedef struct _fileheader fileheader;

/*
 * Holds bitmap's DIB header
 */
struct _bitmapinfoheader
	{
	uint32_t headersize;		//size of headers
    uint32_t width;
    uint32_t height;
    uint16_t planes;			//1
    uint16_t bitsperpixel;
    uint32_t compression;		//no compression
    uint32_t imagesize;			//no compression
    uint32_t ypixelpermeter;	//0 for default resolution
    uint32_t xpixelpermeter;	//0 for default resolution
    uint32_t numcolorspallette;	//colours in palette
    uint32_t mostimpcolor;		//0
	};
typedef struct _bitmapinfoheader bitmapinfoheader;

/*
 * Holds bitmap's headers
 */
struct _headers
	{
	fileheader header1;
	bitmapinfoheader header2;
	};
typedef struct _headers headers;

#pragma pack(pop)

/*
 * Decides which pixel to put on bitmap considering
 * lawn's type of terrain and level of watering
 *
 * FILE *bitmap - must be in "wb" mode
 *
 * char mode - signifies terrain's type
 * 		x - wall/non-grass-terrain (black)
 * 		n - no watering on terrain (white)
 * 		w - terrain was watered (shades of green)
 * 		o - terrain was over-watered (shades of red)
 *
 * uint8_t saturation - signifies shade
 * the lesser the value the lighter the colour
 * 		x/n - any number is fine
 * 		w - must be between 1 and 12
 * 		o - must be between 1 and 16
 *
 * If palette is changed this function must be as well
 * If format is improper puts debug pixel (blue)
 */
void PixelLawn(FILE *bitmap, char mode, uint8_t saturation);

/*
 * Puts pixel on bitmap
 *
 * FILE *bitmap - must be in "wb" mode
 *
 * uint8_t pixel - pixel that will be put on bitmap.
 * It is an index of a colour from a palette.
 */
void PutPixel(FILE *bitmap, uint8_t pixel);

/*
 * Initializes and writes headers and palette to
 * bitmap. Palette can be changed by adding your own
 * fpalette file.
 *
 * FILE *bitmap - must be in "wb" mode
 *
 * returns 1 on success
 * returns 0 on failure
 */
int InitializeBitmap(FILE *bitmap, uint32_t width, uint32_t height);

#endif
