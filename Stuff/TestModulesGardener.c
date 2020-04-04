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

#define JUMP 1

struct _pointlist
	{
	int x;
	int y;
	struct _pointlist *next;
	};
typedef struct _pointlist pointlist;

struct _reclist
	{
	int x1;
	int y1;
	int x2;
	int y2;
	struct _reclist *next;
	};
typedef struct _reclist reclist;

void ResetSignsJump(char **Lawn, int xsize, int ysize)
	{
	for(int j = 0; j < ysize; j++)
		{
		for(int i = 0; i < xsize; i++)
			*(*(Lawn+JUMP*i)+JUMP*j) = -(*(*(Lawn+JUMP*i)+JUMP*j));
		}
	}

void SignAreaJump(char **Lawn, int xsize, int ysize, int i, int j)
	{
	*(*(Lawn+JUMP*i)+JUMP*j) = -(*(*(Lawn+JUMP*i)+JUMP*j));
	if(i + 1 < xsize && *(*(Lawn+JUMP*(i+1))+JUMP*j) == 1)
		SignAreaJump(Lawn, xsize, ysize, i + 1, j);
	if(i - 1 >= 0 && *(*(Lawn+JUMP*(i-1))+JUMP*j) == 1)
		SignAreaJump(Lawn, xsize, ysize, i - 1, j);
	if(j + 1 < ysize && *(*(Lawn+JUMP*i)+JUMP*(j+1)) == 1)
		SignAreaJump(Lawn, xsize, ysize, i, j + 1);
	if(j - 1 >= 0 && *(*(Lawn+JUMP*i)+JUMP*(j-1)) == 1)
		SignAreaJump(Lawn, xsize, ysize, i, j - 1);
	}

pointlist *NewPoint(int x, int y, pointlist *next)
	{
	pointlist *outcome = malloc(sizeof(*outcome));
	if(outcome == NULL)
		return NULL;
	outcome -> x = x;
	outcome -> y = y;
	outcome -> next = next;
	return outcome;
	}

void FreePoints(pointlist *point)
	{
	pointlist *piv;
	while(point != NULL)
		{
		piv = point -> next;
		free(point);
		point = piv;
		}
	}

pointlist *FindAreas(char **Lawn, int xsize, int ysize)
	{
	pointlist *ret = NULL;
	for(int j = 0; j < ysize; j++)
		{
		for(int i = 0; i < xsize; i++)
			{
			if(*(*(Lawn+JUMP*i)+JUMP*j) == 1)
				{
				pointlist *piv = NewPoint(i, j, ret);
				if(piv == NULL)
					{
					FreePoints(ret);
					return NULL;
					}
				ret = piv;
				SignAreaJump(Lawn, xsize, ysize, i, j);
				}
			}
		}
	ResetSignsJump(Lawn, xsize, ysize);
	return ret;
	}

int CalcLenRight(char **Lawn, int x, int y, int xsize)
	{
	int i = x + 1;
	while(i < xsize && *(*(Lawn+JUMP*i)+JUMP*y) != 0)
		i++;
	return i - x;
	}

reclist *InitNewRectangle(reclist *rectangles)
	{
	reclist *piv = malloc(sizeof(*piv));
	if(piv == NULL)
		return NULL;
	while(rectangles -> next != NULL)
		rectangles = rectangles -> next;
	piv -> next = NULL;
	rectangles -> next = piv;
	return piv;
	}

//Might be good to merge this with CalcLenRight
int CalcLenLeft(char **Lawn, int x, int y)
	{
	int i = x - 1;
	while(i >= 0 && *(*(Lawn+JUMP*i)+JUMP*y) != 0)
		i--;
	return x - i;
	}

int CheckRow(char **Lawn, int x, int y, int len, int xsize, int ysize)
	{
	if(y == ysize || y == -1)
		return 1;
	if(x - 1 >= 0 && *(*(Lawn+JUMP*(x-1))+JUMP*y) != 0)
		return 1;
	int i = 0;
	while(i < len)
		{
		if(*(*(Lawn+JUMP*(x+i))+JUMP*y) == 0)
			return 1;
		i++;
		}
	if(x + i < xsize && *(*(Lawn+JUMP*(x+i))+JUMP*y) != 0)
		return 1;
	return 0;
	}

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

void PrintPoints(pointlist *pts)
	{
	while(pts != NULL)
		{
		printf("x = %d, y = %d\n", pts -> x, pts -> y);
		pts = pts -> next;
		}
	}

int CheckForDownUp(char **Lawn, int x1, int x2, int y, int xsize, int ysize, reclist *rectangles)
	{
	int i = x1 - 1;
	if(*(*(Lawn+JUMP*x1)+JUMP*y) != 0)
		{
		while(i >= 0 && *(*(Lawn+JUMP*i)+JUMP*y) != 0)
			{
			if(*(*(Lawn+JUMP*i)+JUMP*(y-1)) != 0)
				{
				rectangles = InitNewRectangle(rectangles);
				if(rectangles == NULL)
					return 0;
				//if(DownUpRectangle(Lawn, i, y-1, xsize, ysize, rectangles) == 0)
					printf("DU: x = %d, y = %d\n", i, y - 1);
					--i;
					while(i >= 0 && *(*(Lawn+JUMP*i)+JUMP*(y-1)) != 0)
						{
						if(*(*(Lawn+JUMP*i)+JUMP*y) == 0)
							i = -4; //break
						--i;
						}
					++i;
				}
			--i;
			}
		}
	int j = x2 + 1;
	if(*(*(Lawn+JUMP*x2)+JUMP*y) != 0)
		{
		while(j < xsize && *(*(Lawn+JUMP*j)+JUMP*y) != 0)
			{
			if(*(*(Lawn+JUMP*j)+JUMP*(y-1)) != 0)
				{
				int newrec = j + 1;
				while(newrec < xsize && *(*(Lawn+JUMP*newrec)+JUMP*(y-1)) != 0)
					++newrec;
				--newrec;
				rectangles = InitNewRectangle(rectangles);
				if(rectangles == NULL)
					return 0;
				//if(DownUpRectangle(Lawn, newrec, y-1, xsize, ysize, rectangles) == 0)
					printf("DU: x = %d, y = %d\n", newrec, y - 1);
					//++j;	HERE - remove all
				while(j <= newrec)
					{
					if(*(*(Lawn+JUMP*j)+JUMP*y) == 0)//HERE
						j = xsize;	//break
					++j;
					}
				}
			++j;
			}
		}
	return 1;
	}

int CheckForUpDown(char **Lawn, int x, int j, int xsize, int ysize, int len, reclist *rectangles)
	{
	int i1 = x + len - 1;
	//int y1 = j;
	while(i1 >= x)
		{
		if(*(*(Lawn+JUMP*i1)+JUMP*j) != 0 && (i1 == 0 || *(*(Lawn+JUMP*(i1-1))+JUMP*j) == 0))
			{
			rectangles = InitNewRectangle(rectangles);
			if(rectangles == NULL)
				return 0;
			//if(UpDownRectangle(Lawn, i1, j, xsize, ysize, rectangles) == 0)
				printf("UD: x = %d, y = %d\n", i1, j);
			}
		--i1;
		}
	if(i1 >= 0 && *(*(Lawn+JUMP*i1)+JUMP*j) != 0 && *(*(Lawn+JUMP*(i1+1))+JUMP*j) != 0)
		{
		while(i1 >= 0 && *(*(Lawn+JUMP*i1)+JUMP*j) != 0)
			--i1;
		++i1;
		rectangles = InitNewRectangle(rectangles);
		if(rectangles == NULL)
			return 0;
		//if(UpDownRectangle(Lawn, i1, j, xsize, ysize, rectangles) == 0)
			printf("UD: x = %d, y = %d\n", i1, j);
		}
	return 1;
	}

int main(void)
	{
	char **Lawn;
	int xsize, ysize;
	FILE *plf = fopen("pseudolawn", "r");	//pseudo-lawn file
	Lawn = ReadLawn(plf, &xsize, &ysize);
	PrintLawn(Lawn, xsize, ysize);
	//pointlist *points = FindAreas(Lawn, xsize, ysize);	//OK
	//PrintPoints(points);
	//int len1 = CalcLenRight(Lawn, 3, 0, xsize);		//OK
	//int len2 = CalcLenLeft(Lawn, 9, 0);			//OK
	//printf("len1 = %d    len2 = %d\n", len1, len2);
	//int cr = CheckRow(Lawn, 0, 2, 10, xsize, ysize);	//OK
	//printf("1 - WRONG | 0 - OK => %d\n", cr);
	//reclist tmp;
	//CheckForUpDown(Lawn, 2, 2, xsize, ysize, 5, &tmp);	//OK
	//CheckForDownUp(Lawn, 2, 6, 2, xsize, ysize, &tmp);	//CHECK BUG FIXES
	return EXIT_SUCCESS;
	}
