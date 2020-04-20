/*
 ============================================================================
 Name        : TestModulesTrawniczek.c
 Author      : liath
 Version     :
 Copyright   : NULL
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include "gardener.h"

char **ReadLawn(FILE *plf, int *xsize, int *ysize)
	{
	fscanf(plf, "%d %d", xsize, ysize);
	int c = getc(plf);//eats \n
	char **Output = malloc(sizeof(*Output) * (*xsize));
	for(int i = 0; i < (*xsize); i++)
		{
		*(Output + i) = malloc(sizeof(**Output) * (*ysize));
		}
	for(int j = 0; j < (*ysize); j++)
		{
		for(int k = 0; k < (*xsize); k++)
			{
			c = getc(plf);
			if(c == '*')
				*(*(Output+JUMP*k)+JUMP*j) = 1;
			else
				*(*(Output+JUMP*k)+JUMP*j) = 0;
			}
		c = getc(plf);
		}
	return Output;
	}

void PrintLawn(char **Lawn, int xsize, int ysize)
	{
	for(int i = 0; i < ysize; i++)
		{
		for(int j = 0; j < xsize; j++)
			{
			if(*(*(Lawn+JUMP*j)+JUMP*i) != 0)
				printf("%d", *(*(Lawn+JUMP*j)+JUMP*i));
			else
				printf("%d", *(*(Lawn+JUMP*j)+JUMP*i));
			}
		printf("\n");
		}
	printf("\n");
	}

//Inside testing version of gardener.c
//void PrintRectangles(reclist *r)

void PrintPoints(pointlist *pts)
	{
	while(pts != NULL)
		{
		printf("x = %d, y = %d\n", pts -> x, pts -> y);
		pts = pts -> next;
		}
	}

void Draw8(char **Lawn, int xc, int yc, int x, int y)
	{
	//*(*(Lawn+xc+x)+yc+y) = 1;
	//*(*(Lawn+xc-x)+yc+y) = 2;
	*(*(Lawn+xc+x)+yc-y) = 3;
	//*(*(Lawn+xc-x)+yc-y) = 4;
	//*(*(Lawn+xc+y)+yc+x) = 5;
	//*(*(Lawn+xc-y)+yc+x) = 6;
	*(*(Lawn+xc+y)+yc-x) = 7;
	//*(*(Lawn+xc-y)+yc-x) = 8;
	}

void DrawCircle(char **Lawn, int r, int xcen, int ycen)
	{
	int x = 0;
	int y = r;
	int d = 3 - (2 * r);
	Draw8(Lawn, xcen, ycen, x, y);
	while(x <= y)
		{
		++x;
		if(d < 0)
			{
			d = d + (4 * x) + 6;
			}
		else
			{
			d = d + 4 * (x - y) + 10;
			--y;
			}
		Draw8(Lawn, xcen, ycen, x, y);
		PrintLawn(Lawn, 17, 17);
		printf("\n");
		}
	}

int main(void)
	{
	char **Lawn;
	int xsize, ysize;
	FILE *plf = fopen("file", "r");	//pseudo-lawn file
	Lawn = ReadLawn(plf, &xsize, &ysize);
	//PrintLawn(Lawn, xsize, ysize);
	//pointlist *points = FindAreas(Lawn, xsize, ysize);	//OK
	//PrintPoints(points);
	//int len1 = CalcLenRight(Lawn, 3, 0, xsize);			//OK
	//int len2 = CalcLenLeft(Lawn, 9, 0);					//OK
	//printf("len1 = %d    len2 = %d\n", len1, len2);
	//int cr = CheckRow(Lawn, 0, 2, 10, xsize, ysize);		//OK
	//printf("1 - WRONG | 0 - OK => %d\n", cr);
	//reclist tmp;
	//CheckForUpDown(Lawn, 8, 2, xsize, ysize, 2, &tmp);	//OK
	//CheckForDownUp(Lawn, 8, 9, 2, xsize, ysize, &tmp);	//OK
	//DoTheJob(Lawn, xsize, ysize);							//OK for now
	//DrawCircle(Lawn, 8, 8, 8);
	PrintLawn(Lawn, xsize, ysize);
	reclist *rectangles = malloc(sizeof(*rectangles));
	UpDownRectangle(Lawn, 0, 0, xsize, ysize, rectangles, rectangles);
	//int Sprstats[4] = {1, 2, 3, 4};
	//char mode = 'h';
	//int type = 3;
	//MakeDecisions(&mode, &type, rectangles, 1);				//OK
	//printf("mode: %c    type: %d\n", mode, type);
	//Lawn, reclist, time, radius, Sprstats[], type - 0, 1, 2, 3
	//PlaceOnYAxis(Lawn, rectangles, 10, 2, 1, 6, Sprstats, 0);	//OK
	//PlaceOnYAxis(Lawn, rectangles, 10, 2, Sprstats, 2);		//OK
	return EXIT_SUCCESS;
	}
