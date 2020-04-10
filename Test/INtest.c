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
	InitializeParameters(lawnfile, &Param, argc, args);
    char **Lawn;
	int errCode;
	errCode = CreateLawn(lawnfile, &Param, &Lawn);
	if (errCode != 0)
		{
		return EXIT_FAILURE;
		}
	fclose(lawnfile);
}
