#include "input.h"

/*
MACROS ARE DEFINED IN INPUT.H

#define maxLawnWidth 8000
#define maxLawnHeigh 4000
#define emp '-' //symbol of the empty space
#define blk 'x' //symbol of the blocked space
#define pxl 100 //size of one square
*/

int CreateLawn(FILE *lawnfile, parameters *Param, char **lawn) //obs³uga b³êdów do ogarniêcia
{
    lawn = malloc(Param->ysize * sizeof *(lawn));
    if (lawn == NULL)
    {
        fprintf(stderr, "Could not allocate memory [Start].\n");
        return -5;
    }
    for (int i = 0; i < Param->ysize; i++)
    {
        lawn[i] = malloc(Param->xsize * sizeof *(*(lawn)));
        if (lawn[i] == NULL)
        {
            fprintf(stderr, "Could not allocate memory [%d].\n", i);
            return -5;
        }
    }
    int x = 0, y = 0; //current positions
    while ((c = fgetc(lawnfile)) != EOF)
    {
        if (c == '\n')
        {
            x = 0;
            y++;
        }
        else if (c == emp)
        {
            Param->nlawn += (Param->pixelsize)*(Param->pixelsize);
            fillSquare(lawn, x, y, 1, Param->pixelsize);
            x++;
        }
        else if (c == blk)
        {
            fillSquare(lawn, x, y, 0, Param->pixelsize);
            x++;
        }
        else
        {
            fprintf(stderr, "Unexpected error.\n");
            return -7;
        }
    }
    return 0;
}

int InitializeParameters(FILE *lawnfile, parameters *Param, int time)
{
    Param->pixelsize = pxl;
    Param->nlawn = 0;
    Param->currentmean = 0;
    Param->radius360 = 200; //
    char c;
    int x = 0, y = 0; //current positions
    int maxX = maxLawnWidth/Param->pixelsize;
    int maxY = maxLawnHeigh/Param->pixelsize;
    while ((c = fgetc(lawnfile)) != EOF)
    {
        if (c == '\n')
        {
            if (y == 0) maxX = x;
            else if (x != maxX)
            {
                fprintf(stderr, "Line %d is too short.\n", y);
                return -100*y-1;
            }
            x = 0;
            y++;
        }
        else if (x >= maxX)
        {
            fprintf(stderr, "Line %d is too long.\n", y);
            return -100*y-2;
        }
        else if (y >= maxY)
        {
            fprintf(stderr, "File has too many lines.\n");
            return -3;
        }
        else if (c == emp || c == blk)
        {
            x++;
        }
        else
        {
            fprintf(stderr, "Unrecognized character in %d line.\n", y);
            return -100*y-4;
        }
    }
    if (y == 0) maxX = x;
    if (maxX == 0)
    {
        fprintf(stderr, "File is empty.\n");
        return -5;
    }
    if (x != maxX && x != 0)
    {
        fprintf(stderr, "Line %d is too short.\n", y);
        return -100*y-1;
    }
    if (x == maxX) y++;
    Param->xsize = Param->pixelsize * x;
    Param->ysize = Param->pixelsize * y;
    Param->time = time;

    return 0;
}

void fillSquare(char **lawn, int x, int y, char c, int pixel)
{
    for (int i = 0; i < pixel; i++)
    {
        for (int j = 0; j < pixel; j++)
        {
            lawn[x*pixel+i][y*pixel+j] = c;
        }
    }
}
