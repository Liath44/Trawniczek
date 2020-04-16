#include "input.h"

int CreateLawn(FILE *lawnfile, parameters *P, char ***lawn)
{
    //allocating 2d array
    char ** temp_lawn = (char **)malloc(P->xsize * sizeof *(temp_lawn));
    if (temp_lawn == NULL)
    {
        fprintf(stderr, "Could not allocate memory [Start].\n");
        return -5;
    }
    for (int i = 0; i < P->xsize; i++)
    {
        temp_lawn[i] = (char *)malloc(P->ysize * sizeof *(*(temp_lawn)));
        if (temp_lawn[i] == NULL)
        {
            fprintf(stderr, "Could not allocate memory [%d].\n", i);
            return -5;
        }
    }
     *lawn = temp_lawn;

    //reading the *lawnfile
    int x = 0, y = 0; //current position
    int maxX = maxLawnWidth/P->pixelsize; //max width is 80
    int maxY = maxLawnHeigh/P->pixelsize; //max height is 40
    char c;
    while ((c = fgetc(lawnfile)) != EOF)
    {
        if (y >= P->ysize/P->pixelsize) //more rows needed
        {
            BiggerLawn(P, lawn);
        }
        if (c == '\n')
        {
            if (y == 0) maxX = x;
            else if (x != maxX)
            {
                fprintf(stderr, "Line %d. is too short.\n", y+1);
                return -100*y-1;
            }
            x = 0;
            y++;
        }
        else if (x >= maxX)
        {
            fprintf(stderr, "Line %d. is too long.\n", y+1);
            return -100*y-2;
        }
        else if (y >= maxY)
        {
            fprintf(stderr, "File has too many lines.\n");
            return -3;
        }
        else if (c == emp)
        {
            if (x >= P->xsize/P->pixelsize) //more columns needed
            {
                BiggerLawn(P, lawn);
            }
            P->nlawn += (P->pixelsize)*(P->pixelsize);
            fillSquare(*lawn, x++, y, 1, P->pixelsize);
        }
        else if (c == blk)
        {
            if (x >= P->xsize/P->pixelsize) //more columns needed
            {
                BiggerLawn(P, lawn);
            }
            fillSquare(*lawn, x++, y, 0, P->pixelsize);
        }
        else
        {
            fprintf(stderr, "Unrecognized character in %d. line.\n", y+1);
            return -100*y-4;
        }
    }
    //check some values before final resizing
    if (y == 0) maxX = x; //if *lawnfile has only one line
    if (maxX == 0) //width == 0
    {
        fprintf(stderr, "File is empty.\n");
        return -5;
    }
    if (x != maxX && x != 0) //last line is different length
    {
        fprintf(stderr, "Line %d. is too short.\n", y+1);
        return -100*y-1;
    }
    if (x == maxX) y++; //if the last line does not have end line character
    ReduceLawn(P, lawn, maxX, y); //remove unused rows and columns
    return 0;
}

void InitializeParameters(parameters *P, int argc, char ** args)
{
    //setting up parameters form args or using default value
    P->pixelsize = pxl;
    P->xsize = 8 * P->pixelsize;
    P->ysize = 4 * P->pixelsize;
    P->nlawn = 0;
    P->time = argc > 2 ? atoi(args[2]) : 100;
    P->radius360 = argc > 3 ? atoi(args[3]) : 200;
    P->currentmean = 0;
}

void fillSquare(char **lawn, int x, int y, char c, int pixel)
{
    for (int j = 0; j < pixel; j++)
    {
        for (int i = 0; i < pixel; i++)
        {
            lawn[y*pixel+i][x*pixel+j] = c;
        }
    }
}

int BiggerLawn(parameters *P, char ***lawn)
{
    //new size is double previous one each direction or max size whichever is smaller
    //first add more rows than resize existing and allocate the new ones
    int temp_ysize = 2*P->ysize < maxLawnHeigh ? 2*P->ysize : maxLawnHeigh; //new number of rows
    int temp_xsize = 2*P->xsize < maxLawnWidth ? 2*P->xsize : maxLawnWidth; //new number of columns
    char ** temp_lawn = (char **) realloc(*lawn, temp_xsize * sizeof *(temp_lawn));
    if (temp_lawn == NULL)
    {
        fprintf(stderr, "Could not allocate memory [realloc].\n");
        return -5;
    }
    *lawn = temp_lawn;
    for (int i = 0; i < P->ysize; i++)
    {
        char * temp_row = (char *) realloc((*lawn)[i], temp_ysize * sizeof *(temp_row));
        if (temp_row == NULL)
        {
            fprintf(stderr, "Could not allocate memory [realloc - %d].\n", i+1);
            return -5;
        }
        (*lawn)[i] = temp_row;
    }
    for (int i = P->ysize; i < temp_xsize; i++)
    {
        char * temp_row = (char *) malloc(temp_ysize * sizeof *(temp_row));
        if (temp_row == NULL)
        {
            fprintf(stderr, "Could not allocate memory [malloc - %d].\n", i+1);
            return -5;
        }
        (*lawn)[i] = temp_row;
    }
    P->ysize = temp_ysize;
    P->xsize = temp_xsize;
}

int ReduceLawn(parameters *P, char ***lawn, int x, int y)
{
    //new size is equal to the number of used columns and rows
    //first remove rows form the end, realloc and then shorten the rest (remove columns)
    int temp_ysize = y * P->pixelsize; //new number of rows
    int temp_xsize = x * P->pixelsize; //new number of columns
    for (int i = temp_xsize; i < P->ysize; i++)
    {
        free((*lawn)[i]);
    }
    char ** temp_lawn = (char **) realloc(*lawn, temp_xsize * sizeof *(temp_lawn));
    if (temp_lawn == NULL)
    {
        fprintf(stderr, "Could not allocate memory [realloc].\n");
        return -5;
    }
    *lawn = temp_lawn;
    for (int i = 0; i < temp_xsize; i++)
    {
        char * temp_row = (char *) realloc((*lawn)[i], temp_ysize * sizeof *(temp_row));
        if (temp_row == NULL)
        {
            fprintf(stderr, "Could not allocate memory [realloc - %d].\n", i+1);
            return -5;
        }
        (*lawn)[i] = temp_row;
    }
    P->ysize = temp_ysize;
    P->xsize = temp_xsize;
}

