#ifndef GARDENER
#define GARDENER

#define JUMP 1

#include <stdlib.h>
#include <stdio.h>
//#include "lawnholder.h"
//#include "output.h"

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
int DoTheJob(char **Lawn, int xsize, int ysize, int time, int radius);
int DownUpRectangle(char **Lawn, int x, int y, int xsize, int ysize, reclist *rectangles, reclist *beg);
int UpDownRectangle(char **Lawn, int x, int y, int xsize, int ysize, reclist *rectangles, reclist *beg);
int CheckForUpDown2(char **Lawn, int x1, int x2, int y, int xsize, int ysize, reclist *rectangles, reclist *beg);
int CheckForDownUp(char **Lawn, int x1, int x2, int y, int xsize, int ysize, reclist *rectangles, reclist *beg);
int CheckForDownUp2(char **Lawn, int x, int j, int xsize, int ysize, int len, reclist *rectangles, reclist *beg);
int CheckForUpDown(char **Lawn, int x, int j, int xsize, int ysize, int len, reclist *rectangles, reclist *beg);
reclist *InitNewRectangle(reclist *rectangles);
int CheckRow(char **Lawn, int x, int y, int len, int xsize, int ysize);
int CalcLenLeft(char **Lawn, int x, int y);
int CalcLenRight(char **Lawn, int x, int y, int xsize);
void FreeRectangles(reclist *rec);
pointlist *FindAreas(char **Lawn, int xsize, int ysize);
void FreePoints(pointlist *point);
pointlist *NewPoint(int x, int y, pointlist *next);
void SignAreaJump(char **Lawn, int xsize, int ysize, int i, int j);
void ResetSignsJump(char **Lawn, int xsize, int ysize);
double FillRecGreedily(char **Lawn, reclist *Rectangles, int time, int nlawn, int radius,
						/*sprlist *Sprinklers,*/ int Sprstats[]);
double PlaceSprGreedily(char **Lawn, reclist *R, double pixmean, int time, double nlawn, int radius,
						/*sprlist *Sprinklers,*/ int Sprstats[], int type);
double PlaceInRectangle(char **Lawn, reclist *R, double pixmean, int time, double nlawn, int radius,
						/*sprlist *Sprinklers,*/ int Sprstats[], int type);
int PlaceSprRow(double *addedval, int y, char **Lawn, reclist *R, int time, double nlawn,
				int radius, /*sprlist *Sprinklers*/ int Sprstats[], int type);
double PlaceOnXAxis(char **Lawn, reclist *R, double pixmean, int time, double nlawn, int radius,
						/*sprlist *Sprinklers*/ int Sprstats[], int type);
double PlaceOnYAxis(char **Lawn, reclist *R, double pixmean, int time, double nlawn, int radius,
						/*sprlist *Sprinklers*/ int Sprstats[], int type);
double PlaceOneMiddle(char **Lawn, reclist *R, double pixmean, int time, double nlawn, int radius,
					/*sprlist *Sprinklers*/ int Sprstats[], int type);
void MakeDecisions(char *mode, int *type, reclist *Rectangles, int radius);
void PrintRectangles(reclist *r);



#endif
