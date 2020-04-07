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
			printf("%d", *(*(Lawn+JUMP*j)+JUMP*i));
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

int main(void)
	{
	char **Lawn;
	int xsize, ysize;
	FILE *plf = fopen("file", "r");	//pseudo-lawn file
	Lawn = ReadLawn(plf, &xsize, &ysize);
	PrintLawn(Lawn, xsize, ysize);
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
	DoTheJob(Lawn, xsize, ysize);	//8
	return EXIT_SUCCESS;
	}
