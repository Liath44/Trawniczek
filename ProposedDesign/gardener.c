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

/*
 * Resets markers on Lawn by multiplying each pixel by -1
 */
void ResetSignsJump(char **Lawn, int xsize, int ysize)
	{
	for(int j = 0; j < ysize; j++)
		{
		for(int i = 0; i < xsize; i++)
			*(*(Lawn+JUMP*i)+JUMP*j) = -(*(*(Lawn+JUMP*i)+JUMP*j));
		}
	}

/*
 * Marks Lawn area enclosed by walls by changing the value
 * to negative. 
 */
void SignAreaJump(char **Lawn, int xsize, int ysize, int i, int j)
	{
	*(*(Lawn+JUMP*i)+JUMP*j) = -(*(*(Lawn+JUMP*i)+JUMP*j));
	//Check recursively for pixels belonging to the area
	if(i + 1 < xsize && *(*(Lawn+JUMP*(i+1))+JUMP*j) == 1)
		SignAreaJump(Lawn, xsize, ysize, i + 1, j);
	if(i - 1 >= 0 && *(*(Lawn+JUMP*(i-1))+JUMP*j) == 1)
		SignAreaJump(Lawn, xsize, ysize, i - 1, j);
	if(j + 1 < ysize && *(*(Lawn+JUMP*i)+JUMP*(j+1)) == 1)
		SignAreaJump(Lawn, xsize, ysize, i, j + 1);
	if(j - 1 >= 0 && *(*(Lawn+JUMP*i)+JUMP*(j-1)) == 1)
		SignAreaJump(Lawn, xsize, ysize, i, j - 1);
	}

/*
 * Creates a point. New points are put at the beginning
 * of the list.
 */
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

/*
 * Creates a list of every enclosed area in a Lawn.
 * 
 * Returns said list on success
 * Returns NULL on failure
 */
pointlist *FindAreas(char **Lawn, int xsize, int ysize)
	{
	pointlist *ret = NULL;
	for(int j = 0; j < ysize; j++)
		{
		for(int i = 0; i < xsize; i++)
			{
			if(*(*(Lawn+JUMP*i)+JUMP*j) == 1)
				{
				//Found new area
				pointlist *piv = NewPoint(i, j, ret);
				if(piv == NULL)
					{
					FreePoints(ret);
					return NULL;
					}
				ret = piv;
				//Mark area by making it's pixels negative
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
/*
 * Calculates considered rectangle length from
 * left to right.
 */
int CalcLenRight(char **Lawn, int x, int y, int xsize)
	{
	//First pixel is always lawn
	int i = x + 1;
	while(i < xsize && *(*(Lawn+JUMP*i)+JUMP*y) != 0)
		i++;
	return i - x;
	}

/*
 * Calculates considered rectangle length from
 * right to left.
 */
int CalcLenLeft(char **Lawn, int x, int y)
	{
	//First pixel is always lawn
	int i = x - 1;
	while(i >= 0 && *(*(Lawn+JUMP*i)+JUMP*y) != 0)
		i--;
	return x - i;
	}

/*
 * Checks wheher considered row should still be
 * considered a part of subject rectangle or not
 */
int CheckRow(char **Lawn, int x, int y, int len, int xsize, int ysize)
	{
	if(y == ysize || y == -1)
		return 1;
	//Is row's left side wall?
	if(x - 1 >= 0 && *(*(Lawn+JUMP*(x-1))+JUMP*y) != 0)
		return 1;
	//Are pixels inside row lawn?
	int i = 0;
	while(i < len)
		{
		if(*(*(Lawn+JUMP*(x+i))+JUMP*y) == 0)
			return 1;
		i++;
		}
	//Is row's right side wall?
	if(x + i < xsize && *(*(Lawn+JUMP*(x+i))+JUMP*y) != 0)
		return 1;
	return 0;
	}

/*
 * Allocates memory for a new rectangle and
 * puts it at the end of list.
 * 
 * Returns created rectangle on success
 * Returns NULL on failure
 */
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
/*
 * After examining subject rectangle check for 
 * other rectangles that could be examined
 * from top to bottom (@int:UpDownRectangle(...))
 * 
 * char **Lawn - matrix that stores lawn's pixels
 * 
 * int x - x coordinate of a right-down corner of 
 * subject rectangle 
 * 
 * int j - y coordinate of a row in which to search
 * for new rectangles
 * 
 * int xsize, ysize - Lawn's size
 * 
 * int len - length of subject rectangle
 * 
 * reclist *rectangles - points to a certain part 
 * of list of already found rectangles
 * 
 * Returns 0 on memory allocation error
 * Returns 1 otherwise
 */
int CheckForUpDown(char **Lawn, int x, int j, int xsize, int ysize, int len, reclist *rectangles)
	{
	//x coordinate of left-down corner
	int i1 = x + len - 1;
	//Check for UpDowns directly below rectangle
	while(i1 >= x)
		{
		//New rectangle when it's left wall is found
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
	//Check for UpDowns to the left of the rectangle
	//The passage on j row must be clear
	if(i1 >= 0 && *(*(Lawn+JUMP*i1)+JUMP*j) != 0 && *(*(Lawn+JUMP*(i1+1))+JUMP*j) != 0)
		{
		//New rectangle when it's left wall is found
		//Only one rectangle possible in this scenario
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

/*
 * Analogical to CheckForDownUp but called
 * by DownUpRectangle
 */
int CheckForDownUp2(char **Lawn, int x, int y, int xsize, int ysize, int len, reclist *rectangles)
	{
	////////////
	return 1;
	}

/*
 * After examining subject rectangle check for 
 * other rectangles that could be examined
 * from bottom to top (@int:UpDownRectangle(...))
 * 
 * char **Lawn - matrix that stores lawn's pixels
 * 
 * int x1 - x coordinate of a left-down corner of 
 * subject rectangle 
 * 
 * int x2 - x coordinate of a right-down corner of 
 * subject rectangle
 * 
 * int y - y coordinate a row above which new 
 * rectangles should be searched for
 * 
 * int xsize, ysize - Lawn's size
 * 
 * reclist *rectangles - points to a certain part 
 * of list of already found rectangles
 * 
 * Returns 0 on memory allocation error
 * Returns 1 otherwise
 */
int CheckForDownUp(char **Lawn, int x1, int x2, int y, int xsize, int ysize, reclist *rectangles)
	{
	//Check for DownUps to the left of the rectangle 
	int i = x1 - 1;
	if(*(*(Lawn+JUMP*x1)+JUMP*y) != 0)
		{
		//While row is still lawn
		while(i >= 0 && *(*(Lawn+JUMP*i)+JUMP*y) != 0)
			{
			//If pixel above lawn pixel is also lawn
			if(*(*(Lawn+JUMP*i)+JUMP*(y-1)) != 0)
				{
				rectangles = InitNewRectangle(rectangles);
				if(rectangles == NULL)
					return 0;
				if(DownUpRectangle(Lawn, i, y-1, xsize, ysize, rectangles) == 0)
					return 0;
				//Go left 
				--i;
				while(i >= 0 && *(*(Lawn+JUMP*i)+JUMP*(y-1)) != 0)
					{
					if(*(*(Lawn+JUMP*i)+JUMP*y) == 0)
						i = -4;
					--i;
					}
				++i;
				}
			--i;
			}
		}
	//Check for DownUps to the right of the rectangle 
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

/*
 * Analogical to CheckForUpDown but called
 * by DownUpRectangle
 */
int CheckForUpDown2(char **Lawn, int x1, int x2, int y, int xsize, int ysize, reclist *rectangles)
	{
	///////////////
	return 1;
	}

/*
 * Function finds a rectangle which:
 *  - is defined by two points one of which
 * is (x, y)
 *  - has the longest length possible
 *  - is enclosed by walls from left & right
 * The search is done from top-left corner to 
 * down-right corner.
 * 
 * char **Lawn - matrix that stores lawn's pixels
 * 
 * int x, y - coordinates of top-left corner
 * 
 * int xsize, ysize - Lawn's size
 * 
 * reclist *rectangles - stores to-be-found 
 * parameters of a rectangle
 * 
 * Returns 0 on memory allocation errors
 * Returns 1 otherwise
 */
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

/*
 * Analogical to UpDownRectangle but the
 * search is done from down-right corner 
 * to up-left corner
 */ 
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
