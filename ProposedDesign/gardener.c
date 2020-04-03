#include "gardener.h"
//stdlib, lawnholder, output

/*
	TODO:
 - FIX x1, y1, x2, y2
 - IF ENTIRE LAWN IS WALL DO NOTHING BUT NO ERROR (?) 
 - "MERGE" SOME FUNCTIONS
 - IS NULL LAWN CHECKED?
	TODO:
*/

/*
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
	}
typedef struct _reclist reclist;
*/

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

void FreeRectangles(reclist *rec)
	{
	reclist *piv;
	while(rec != NULL)
		{
		piv = rec -> next;
		free(rec);
		rec = piv;
		}
	}

int CalcLenRight(char **Lawn, int x, int y, int xsize)
	{
	int i = x + 1;
	while(i < xsize && *(*(Lawn+JUMP*i)+JUMP*y) != 0)
		i++;
	return i - x;
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
			if(UpDownRectangle(Lawn, i1, j, xsize, ysize, rectangles) == 0)
				return 0;
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
		if(UpDownRectangle(Lawn, i1, j, xsize, ysize, rectangles) == 0)
			return 0;
		}
	return 1;
	}

int CheckForDownUp2(char **Lawn, int x, int y, int xsize, int ysize, int len, reclist *rectangles)
	{
	////////////
	return 1;
	}

//y-1 zawsze nie będzie < 0
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
				if(DownUpRectangle(Lawn, i, y-1, xsize, ysize, rectangles) == 0)
					return 0;
				--i;
				while(i >= 0 && *(*(Lawn+JUMP*i)+JUMP*(y-1)) != 0)
					--i;
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
				if(DownUpRectangle(Lawn, newrec, y-1, xsize, ysize, rectangles) == 0)
					return 0;
				while(j <= newrec)
					{
					if(*(*(Lawn+JUMP*j)+JUMP*y) == 0)
						j = xsize;	//break
					++j;				
					}
				}
			++j;
			}
		}
	return 1;
	}

int CheckForUpDown2(char **Lawn, int x1, int x2, int y, int xsize, int ysize, reclist *rectangles)
	{
	///////////////
	return 1;
	}

int UpDownRectangle(char **Lawn, int x, int y, int xsize, int ysize, reclist *rectangles)
	{
	rectangles -> x1 = x;	//FIX
	rectangles -> y1 = y;	//FIX
	int len = CalcLenRight(Lawn, x, y, xsize);
	int j = y + 1;
	int isend = CheckRow(Lawn, x, j, len, xsize, ysize);
	while(isend == 0)
		{
		j++;
		isend = CheckRow(Lawn, x, j, len, xsize, ysize);
		}
	rectangles -> x2 = x + len - 1;	//FIX
	rectangles -> y2 = j - 1;	//FIX
	if(j < ysize)
		{
		if(CheckForUpDown(Lawn, x, j, xsize, ysize, len, rectangles) == 0)
			return 0;
		if(CheckForDownUp(Lawn, x, x + len - 1, j, xsize, ysize, rectangles) == 0)
			return 0;
		}
	return 1;
	}

int DownUpRectangle(char **Lawn, int x, int y, int xsize, int ysize, reclist *rectangles)
	{
	rectangles -> x2 = x;	//FIX
	rectangles -> y2 = y;	//FIX
	int len = CalcLenLeft(Lawn, x, y);
	int j = y - 1;
	int isend = CheckRow(Lawn, x - len + 1, j, len, xsize, ysize);
	while(isend == 0)
		{
		j--;
		isend = CheckRow(Lawn, x - len + 1, j, len, xsize, ysize);
		}
	rectangles -> x1 = x - len + 1;	//FIX
	rectangles -> y1 = j + 1;	//FIX
	if(j >= 0)
		{
		if(CheckForUpDown2(Lawn, x - len + 1, x, j, xsize, ysize, rectangles) == 0)
			return 0;
		if(CheckForDownUp2(Lawn, x - len + 1, j, xsize, ysize, len, rectangles) == 0)
			return 0;
		}
	return 1;
	}

int DoTheJob(char **Lawn, parameters *Param, sprlist *Sprinklers);
	{
	int errcode = 1;
	pointlist *areas = FindAreas(Lawn, Param->xsize/JUMP, Param->ysize/JUMP);
	pointlist *pivareas = areas;
	if(areas == NULL)
		return 0;
	while(areas != NULL)
		{
		reclist *rectangles = malloc(sizeof(*rectangles));
		if(rectangles == NULL)
			{
			FreePoints(pivareas);
			return 0;
			}
		rectangles -> next = NULL;
		errcode = UpDownRectangle(Lawn, areas->x, areas->y, Param->xsize/JUMP, Param->ysize/JUMP, rectangles);
		if(errcode == 0)
			{
			FreeRectangles(rectangles);
			FreePoints(pivareas);
			return 0;
			}
		//
		FreeRectangles(rectangles);
		areas = areas -> next;
		}
	FreePoints(pivareas);
	return 1;	
	}
