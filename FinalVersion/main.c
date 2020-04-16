#include <stdio.h>
#include <stdlib.h>
#include "input.h"
#include "lawnholder.h"
#include "gardener.h"
#include "output.h"

void FreeLawn(FILE *lawnfile, char **Lawn, parameters *Param)
	{
	fclose(lawnfile);
	for(int i = 0; i < Param->xsize; ++i)
		free(*(Lawn+i));
	free(Lawn);
	}

void FreeMemory(sprlist *Sprinklers, FILE *lawnfile, char **Lawn, parameters *Param, FILE *bitmap, FILE *output)
	{
	FreeSprinklers(Sprinklers);
	FreeLawn(lawnfile, Lawn, Param);
	fclose(bitmap);
	fclose(output);
	}

int main(int argc, char *args[])
	{
	if(argc <= 1)
        {
		fprintf(stderr, "No input file.\n");
		return EXIT_FAILURE;
        }
    FILE *lawnfile = fopen(args[1], "r");
    if(lawnfile == NULL)
		{
		fprintf(stderr, "Couldn't open input file.\n");
		return EXIT_FAILURE;
		}
    parameters Param;
	InitializeParameters(&Param, argc, args);
    char **Lawn;
	int errCode;
	errCode = CreateLawn(lawnfile, &Param, &Lawn);
	if (errCode != 0)
		{
		return EXIT_FAILURE;
		}

	sprlist Sprinklers;
	if(DoTheJob(Lawn, &Param, &Sprinklers) == 0)
		{
		FreeLawn(lawnfile, Lawn, &Param);
		FreeSprinklers(&Sprinklers);
		fprintf(stderr, "The Job could not be done\n");
		return EXIT_FAILURE;
		}

	FILE *bitmap = fopen("BITMAP.bmp", "wb");
	FILE *output = fopen("SPRINKLERSPLACEMENT", "w");
	if(bitmap == NULL || output == NULL)
		{
		FreeMemory(&Sprinklers, lawnfile, Lawn, &Param, bitmap, output);
		fprintf(stderr, "Could not open output files\n");
		return EXIT_FAILURE;
		}
	if(CreateBitmap(bitmap, Lawn, &Param) == 0)
		{
		FreeMemory(&Sprinklers, lawnfile, Lawn, &Param, bitmap, output);
		fprintf(stderr, "Could not create bitmap\n");
		return EXIT_FAILURE;
		}
	CreateOutputFile(output, &Sprinklers);

	FreeMemory(&Sprinklers, lawnfile, Lawn, &Param, bitmap, output);
	return EXIT_SUCCESS;
	}
