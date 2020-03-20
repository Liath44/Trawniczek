#include <stdio.h>
#include <stdlib.h>
#include "save_lawn.h"

int save_lawn(FILE *f, char **lawn, int height, int width)
{
    char n = '\n';
	int c;
	char temp;
	int ii = 0;
	int j = 0;
	while(c != EOF)
    {
        c = fgetc(f);
        temp = (char)c;
            if(temp != n)
            {
                if(j<width)  //if \n doesn't occur too early
                {
                    if(temp == 'x' || temp == '-')
                    {
                        lawn[ii][j] = temp;
                        j++;
                    }
                    else if(temp == EOF) //EOF occurs if last line is too short
                    {
                        fprintf(stderr, "Error occurred: lawn not wide enough [%d][%d]", ii+1,j+1);
                        return -1;
                    }
                    else
                    {
                        fprintf(stderr, "Error occurred: wrong syntax [%d][%d]", ii+1,j+1);
                        return -2;
                    }
                }
                else
                {
                    if(temp != EOF) //if j == width then temp == EOF
                    {
                    fprintf(stderr, "Error occurred: lawn is too wide [%d][%d]", ii+1,j+1);
                    return -3;
                    }
                }
            }
            else
            {
                if(j==width) //\n should occur
                {
                    if(ii < height-1) //height -1 to omit writing outside the bounds of the array
                    {
                        ii++;
                        j=0;
                    }
                    else
                    {
                    fprintf(stderr, "Error occurred: lawn is too long [%d][%d]", ii+2,j-3);
                    return -4;
                    }
                }
                else //only option for \n is if j<width
                {
                    fprintf(stderr, "Error occurred: lawn not wide enough [%d][%d]", ii+1,j+1);
                    return -5;
                }
            }
    }
    if(ii < height-1) //check if lawn is too short
    {
        fprintf(stderr, "Error occurred: lawn not long enough [%d][%d]", ii+2,j-4);
        return -6;
    }

	for(ii=0; ii<height; ii++)
    {
        for(j=0; j<width; j++)
        {
            printf("%c", lawn[ii][j]);
        }
        printf("\n");
    }
    return 0;
}
/*
//no malloc
int save_lawn(FILE *f, int height, int width, char lawn[height][width])
{
    char n = '\n';
	int c;
	char temp;
	int ii = 0;
	int j = 0;
	while(c != EOF)
    {
        c = fgetc(f);
        temp = (char)c;
            if(temp != n)
            {
                if(j<width)  //if \n doesn't occur too early
                {
                    if(temp == 'x' || temp == '-')
                    {
                        lawn[ii][j] = temp;
                        j++;
                    }
                    else if(temp == EOF) //EOF occurs if last line is too short
                    {
                        fprintf(stderr, "Error occurred: lawn not wide enough [%d][%d]", ii+1,j+1);
                        return -1;
                    }
                    else
                    {
                        fprintf(stderr, "Error occurred: wrong syntax [%d][%d]", ii+1,j+1);
                        return -2;
                    }
                }
                else
                {
                    if(temp != EOF) //if j == width then temp == EOF
                    {
                    fprintf(stderr, "Error occurred: lawn is too wide [%d][%d]", ii+1,j+1);
                    return -3;
                    }
                }
            }
            else
            {
                if(j==width) //\n should occur
                {
                    if(ii < height-1) //height -1 to omit writing outside the bounds of the array
                    {
                        ii++;
                        j=0;
                    }
                    else
                    {
                    fprintf(stderr, "Error occurred: lawn is too long [%d][%d]", ii+2,j-3);
                    return -4;
                    }
                }
                else //only option for \n is if j<width
                {
                    fprintf(stderr, "Error occurred: lawn not wide enough [%d][%d]", ii+1,j+1);
                    return -5;
                }
            }
    }
    if(ii < height-1) //check if lawn is too short
    {
        fprintf(stderr, "Error occurred: lawn not long enough [%d][%d]", ii+2,j-4);
        return -6;
    }

	for(ii=0; ii<height; ii++)
    {
        for(j=0; j<width; j++)
        {
            printf("%c", lawn[ii][j]);
        }
        printf("\n");
    }
    return 0;
}
*/
