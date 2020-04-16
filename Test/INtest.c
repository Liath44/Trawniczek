#include <stdio.h>
#include <stdlib.h>
#include "input.h"
#include "lawnholder.h"

int main(int argc, char ** args)
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
	fclose(lawnfile);

	//check
	printf("xsize = %d\n", Param.xsize);
	printf("ysize = %d\n", Param.ysize);
	printf("pixelsize = %d\n", Param.pixelsize);
	printf("nlawn = %d\n", Param.nlawn);
	printf("time = %d\n", Param.time);
	printf("radius360 = %d\n", Param.radius360);
	printf("currentmean = %f\n", Param.currentmean);

	for (int j = 0; j < Param.ysize; j+=10)
    {
        for (int i = 0; i < Param.xsize; i+=10)
        {
            printf("%d ", Lawn[i][j]);
        }
        printf("\n");
    }
}

