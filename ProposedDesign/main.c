#include <stdio.h>
#include <stdlib.h>
#include "input.h"
#include "lawnholder.h"
#include "gardener.h"
#include "output.h"

void FreeMemory(FILE *lawnfile, lawn *Lawn)
	{
	fclose(lawnfile);
	//(...)
	}

int main(int argc, char *args[])
	{
	if(argc <= 1)
		{
		fprintf(stderr, "No input file\n");
		return EXIT_FAILURE;
		}

	FILE *lawnfile = fopen(args[1], "r");
	if(lawnfile == NULL)
		{
		fprintf(stderr, "Couldn't open input file\n");
		return EXIT_FAILURE;
		}
	
	lawn Lawn;
	parameters Param;
	InitializeParameters(lawnfile, &Param)
	if(CreateLawn(lawnfile, &Lawn, &Param) == 0)
		{
		FreeMemory(lawnfile, &Lawn);
		fprintf(stderr, "Memory allocation error\n");
		return EXIT_FAILURE;
		}
	
	sprlist Sprinklers;
	if(DoTheJob(&Lawn, &Param, &Sprinklers) == 0)
		{
		FreeMemory(lawnfile, &Lawn);
		fprintf(stderr, "The Job could not be done\n");
		return EXIT_FAILURE;
		}

	FILE *bitmap = fopen("BITMAP.bmp", "wb");
	FILE *output = fopen("SPRINKLERSPLACEMENT", "w");
	if(bitmap == NULL || output == NULL)
		{
		FreeMemory(lawnfile, &Lawn);
		fclose(bitmap);
		fclose(output);
		fprintf(stderr, "Could not open output files\n");
		return EXIT_FAILURE;
		}	
	if(CreateBitmap(bitmap, &Lawn, &Param) == 0)
		{
		FreeMemory(lawnfile, &Lawn);
		fclose(bitmap);
		fclose(output);
		fprintf(stderr, "Could not create bitmap\n");
		return EXIT_FAILURE;
		}
	CreateOutputFile(output, &Sprinklers);

	return EXIT_SUCCESS;
	}
