#include <stdio.h>
#include <stdlib.h>
#define height 4
#define width 8

int main(int argc, char **argv)
{
	FILE* f;
	f = fopen(argv[1], "r");
	if(f == NULL)
    {
            fprintf(stderr, "Error occurred: file open failed");
            return -10;
	}
	int errorcode;
	char **lawn;
	lawn = malloc(height*sizeof(*lawn));
	if(lawn == NULL)
    {
            fprintf(stderr, "Error occurred: could not allocate memory");
            return -11;
	}
	int i;
	for(i=0; i<width; i++)
    {
        lawn[i] = malloc(width*sizeof(*lawn[0]));
        	if(lawn[i] == NULL)
            {
                fprintf(stderr, "Error occurred: could not allocate memory[%d]",i);
                return -12;
            }
    }
    errorcode = save_lawn(f, lawn, height, width);
    fclose(f);

    for(i =0;i<width; i++)
    {
        free(lawn[i]);
    }
    free(lawn);

	return errorcode;
}

/*
//no malloc
#include <stdio.h>
#include <stdlib.h>
#define height 4
#define width 8

int main(int argc, char **argv)
{
	FILE* f;
	f = fopen(argv[1], "r");
	if(f == NULL)
    {
            fprintf(stderr, "Error occurred: file open failed");
            return -10;
	}
	int errorcode;
	char lawn[height][width];

    errorcode = save_lawn(f, height, width, lawn);

    fclose(f);

	return errorcode;
}
*/
