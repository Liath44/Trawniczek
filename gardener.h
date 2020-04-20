#ifndef GARDENER
#define GARDENER

#include <stdlib.h>
#include "lawnholder.h"
#include "output.h"
#include "input.h"

//Macro pxl is defined in input.h
#define JUMP pxl
//macros not used in current version
#define MAXSPR 500
#define STATERR 0.8
#define BOUNCEERR 0.75

/*
 * List of points 
 * Points are used to mark Areas
 * 
 * int x, y - coordinates
 * 
 * struct _pointlist *next - next point on list
 */
struct _pointlist
	{
	int x;
	int y;
	struct _pointlist *next;
	};
typedef struct _pointlist pointlist;

/*
 * List of rectangles (defined by two points)
 * 
 * int x1, y1 - up-left corner of rectangle
 * 
 * int x2, y2 - down-right corner of rectangle
 * 
 * struct _reclist *next - next rectangle on list
 */
struct _reclist
	{
	int x1;
	int y1;
	int x2;
	int y2;
	struct _reclist *next;
	};
typedef struct _reclist reclist;

//structure is not used in current version
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
