#include "input.h"
#define maxLawnWidth 8000
#define maxLawnHeigh 4000
#define emp '-' //symbol of the empty space
#define blk 'x' //symbol of the blocked space
#define pxl 100 //size of one square

int CreateLawn(FILE *lawnfile, parameters *P, char ***lawn)
{
    char ** temp_lawn = malloc(P->ysize * sizeof *(temp_lawn));
    if (temp_lawn == NULL)
    {
        fprintf(stderr, "Could not allocate memory [Start].\n");
        return -5;
    }
    for (int i = 0; i < P->ysize; i++)
    {
        temp_lawn[i] = malloc(P->xsize * sizeof *(*(temp_lawn)));
        if (temp_lawn[i] == NULL)
        {
            fprintf(stderr, "Could not allocate memory [%d].\n", i);
            return -5;
        }
    }
    *lawn = temp_lawn;

    int x = 0, y = 0; //current position
    int maxX = maxLawnWidth/P->pixelsize;
    int maxY = maxLawnHeigh/P->pixelsize;
    char c;
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
        else if (c == emp)
        {
            Param->nlawn += (Param->pixelsize)*(Param->pixelsize);
            fillSquare(lawn, x++, y, 1, P->pixelsize);
            if (x > P->xsize/P->pixelsize)
            {
                BiggerLawn(&P, &lawn);
            }
        }
        else if (c == blk)
        {
            fillSquare(lawn, x++, y, 0, P->pixelsize);
            if (x > P->xsize/P->pixelsize)
            {
                BiggerLawn(&P, &lawn);
            }
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
    ReduceLawn(&P, &lawn, maxX, y);
    return 0;
}

void InitializeParameters(FILE *lawnfile, parameters *P, int argc, char ** args)
{
    P->pixelsize = 100;
    P->xsize = 8 * P->pixelsize;
    P->ysize = 4 * P->pixelsize;
    P->nlawn = 0;
    P->time = argc > 2 ? args[2] : 100;
    P->radius360 = argc > 3 ? args[3] : 200;
    P->currentmean = 0;
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

int BiggerLawn(parameters *P, char ***lawn)
{
    int temp_ysize = min(2*P->ysize, maxLawnHeigh); //new number of rows
    int temp_xsize = min(2*P->xsize, maxLawnWidth); //new number of columns
    char ** temp_lawn = realloc(*lawn, temp_ysize * sizeof *(temp_lawn));
    if (temp_lawn == NULL)
    {
        fprintf(stderr, "Could not allocate memory [realloc].\n");
        return -5;
    }
    *lawn = temp_lawn;
    for (int i = 0; i < P->ysize; i++)
    {
        char * temp_row = realloc(*lawn[i], temp_xsize * sizeof *(temp_row));
        if (temp_row == NULL)
        {
            fprintf(stderr, "Could not allocate memory [realloc - %d].\n", i);
            return -5;
        }
        *lawn[i] = temp_row;
    }
    for (int i = P->ysize; i < temp_ysize; i++)
    {
        char * temp_row = malloc(temp_xsize * sizeof *(temp_row));
        if (temp_row == NULL)
        {
            fprintf(stderr, "Could not allocate memory [malloc - %d].\n", i);
            return -5;
        }
        *lawn[i] = temp_row;
    }
    P->ysize = temp_ysize;
    P->xsize = temp_xsize;
}

int ReduceLawn(parameters *P, char ***lawn, int x, int y)
{
    int temp_ysize = y * P->pixelsize; //new number of rows
    int temp_xsize = x * P->pixelsize; //new number of columns
    for (int i = temp_ysize; i < P->ysize; i++)
    {
        free(*lawn[i]);
    }
    char ** temp_lawn = realloc(*lawn, temp_ysize * sizeof *(temp_lawn));
    if (temp_lawn == NULL)
    {
        fprintf(stderr, "Could not allocate memory [realloc].\n");
        return -5;
    }
    *lawn = temp_lawn;
    for (int i = 0; i < temp_ysize; i++)
    {
        char * temp_row = realloc(*lawn[i], temp_xsize * sizeof *(temp_row));
        if (temp_row == NULL)
        {
            fprintf(stderr, "Could not allocate memory [malloc - %d].\n", i);
            return -5;
        }
        *lawn[i] = temp_row;
    }
    P->ysize = temp_ysize;
    P->xsize = temp_xsize;
}

