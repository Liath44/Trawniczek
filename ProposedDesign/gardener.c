#include "gardener.h"
//stdlib, lawnholder, output

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

pointlist *NewPoint()

pointlist *FindAreas(char **Lawn, int xsize, int ysize)
	{
	pointlist *ret = NULL
	for(int j = 0; j < ysize; j++)
		{
		for(int i = 0; i < xsize; i++)
			{
			if(*(*(Lawn+JUMP*i)+JUMP*j) == 1)
				{
				pointlist *piv = NewPoint(i, j, ret);
				if(piv == NULL)
					return NULL;
				ret = piv;
				SignArea(Lawn, xsize, ysize, i, j);
				}
			}
		}
	ResetSigns(Lawn, xsize, ysize);
	return ret;
	}

int DoTheJob(char **Lawn, parameters *Param, sprlist *Sprinklers);
	{
	pointlist *areas = FindAreas(Lawn, Param->xsize/JUMP, Param->ysize/JUMP);
	if(areas == NULL)
		return 0;
	
	}
