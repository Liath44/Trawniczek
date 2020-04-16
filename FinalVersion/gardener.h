#ifndef GARDENER
#define GARDENER

#include <stdlib.h>
#include "lawnholder.h"
#include "output.h"
#include "input.h"

//Macro pxl is defined in input.h
#define JUMP 100
#define MAXSPR 500
#define STATERR 0.8
#define BOUNCEERR 0.75

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

struct _areatowater
	{
	int x;
	int y;
	int pixcount;
	int xmax;
	int xmin;
	int ymax;
	int ymin;
	int besttype;
	int bestdeg;
	int bestscore;
	};
typedef struct _areatowater areatowater;

/*
 * Places sprinklers on lawn in order to water it
 * 
 * char **Lawn - matrix that stores lawn's pixels
 * 
 * parameters *Param - points to structure that holds 
 * lawn's parameters
 * 
 * sprlist *Sprinklers - list of to-be-placed placed sprinklers
 * 
 * Returns 0 on failure
 */
int DoTheJob(char **Lawn, parameters *Param, sprlist *Sprinklers);
int DownUpRectangle(char **Lawn, int x, int y, int xsize, int ysize, reclist *Rectangles, reclist *Beg);
int UpDownRectangle(char **Lawn, int x, int y, int xsize, int ysize, reclist *Rectangles, reclist *Beg);

#endif
