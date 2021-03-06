#include "gardener.h"
//stdlib, lawnholder, output

/*
	TODO:
 - FIX x1, y1, x2, y2
 - IF ENTIRE LAWN IS WALL DO NOTHING BUT NO ERROR (?) 
 - "MERGE" SOME FUNCTIONS
 - IS NULL LAWN CHECKED?
 - CHECKING WHETHER RECTANGLE WAS FOUND IS NECCESSARY ***
                                                      *-*
                                                      ***					
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
 * 
 * Returns NULL on memory allocation error
 * Returns created point otherwise
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
 * 
 * Returns 1 if row is improper
 * Returns 0 otherwise
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
 * Checks whether point (x, y) is inside one
 * of rectangles from reclist *rl
 *
 * Returns 1 if true
 * Returns 0 otherwise
 */
int IsOnReclist(int x, int y, reclist *rl)
	{
	while(rl != NULL)
		{
		if(x >= rl->x1 && y >= rl->y1 && x <= rl->x2 && y <= rl->y2)
			return 1;
		rl = rl -> next;
		}
	return 0;
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
int CheckForUpDown(char **Lawn, int x, int j, int xsize, int ysize, int len, reclist *rectangles, reclist *beg)
	{
	//x coordinate of left-down corner
	int i1 = x + len - 1;
	//Check for UpDowns directly below rectangle
	while(i1 >= x)
		{
		//New rectangle when it's left wall is found
		if(*(*(Lawn+JUMP*i1)+JUMP*j) != 0 && (i1 == 0 || *(*(Lawn+JUMP*(i1-1))+JUMP*j) == 0))
			{
			if(IsOnReclist(i1, j, beg) == 0)
				{
				rectangles = InitNewRectangle(rectangles);
				if(rectangles == NULL)
					return 0;
				if(UpDownRectangle(Lawn, i1, j, xsize, ysize, rectangles, beg) == 0)
					return 0;
				}
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
		if(IsOnReclist(i1, j, beg) == 0)
			{
			rectangles = InitNewRectangle(rectangles);
			if(rectangles == NULL)
				return 0;
			if(UpDownRectangle(Lawn, i1, j, xsize, ysize, rectangles, beg) == 0)
				return 0;
			}
		}
	return 1;
	}

/*
 * Analogical to CheckForDownUp but called
 * by DownUpRectangle
 */
int CheckForDownUp2(char **Lawn, int x, int j, int xsize, int ysize, int len, reclist *rectangles, reclist *beg)
	{
	//x coordinate of right-up corner
	int i1 = x - len + 1;
	//Check for DownUps directly above rectangle
	while(i1 <= x)
		{
		//New rectangle when it's right wall is found
		if(*(*(Lawn+JUMP*i1)+JUMP*j) != 0 && (i1 == xsize-1 || *(*(Lawn+JUMP*(i1+1))+JUMP*j) == 0))
			{
			if(IsOnReclist(i1, j, beg) == 0)
				{
				rectangles = InitNewRectangle(rectangles);
				if(rectangles == NULL)
					return 0;
				if(DownUpRectangle(Lawn, i1, j, xsize, ysize, rectangles, beg) == 0)
					return 0;
				}
			}
		++i1;
		}
	//Check for DownUps to the right of the rectangle
	//The passage on j row must be clear
	if(i1 < xsize && *(*(Lawn+JUMP*i1)+JUMP*j) != 0 && *(*(Lawn+JUMP*(i1-1))+JUMP*j) != 0)
		{
		//New rectangle when it's right wall is found
		//Only one rectangle possible in this scenario
		while(i1 < xsize && *(*(Lawn+JUMP*i1)+JUMP*j) != 0)
			++i1;
		--i1;
		if(IsOnReclist(i1, j, beg) == 0)
			{
			rectangles = InitNewRectangle(rectangles);
			if(rectangles == NULL)
				return 0;
			if(DownUpRectangle(Lawn, i1, j, xsize, ysize, rectangles, beg) == 0)
				return 0;
			}
		}
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
int CheckForDownUp(char **Lawn, int x1, int x2, int y, int xsize, int ysize, reclist *rectangles, reclist *beg)
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
				if(IsOnReclist(i, y-1, beg) == 0)
					{
					rectangles = InitNewRectangle(rectangles);
					if(rectangles == NULL)
						return 0;
					if(DownUpRectangle(Lawn, i, y-1, xsize, ysize, rectangles, beg) == 0)
						return 0;
					}
				//Check below new rectangle if still lawn
				--i;
				while(i >= 0 && *(*(Lawn+JUMP*i)+JUMP*(y-1)) != 0)
					{
					if(*(*(Lawn+JUMP*i)+JUMP*y) == 0)
						i = -4; //Found wall - break
					--i;
					}
				//Counter upcoming --i;
				++i;
				}
			--i;
			}
		}
	//Check for DownUps to the right of the rectangle 
	int j = x2 + 1;
	if(*(*(Lawn+JUMP*x2)+JUMP*y) != 0)
		{
		//While row is still lawn
		while(j < xsize && *(*(Lawn+JUMP*j)+JUMP*y) != 0)
			{
			//If pixel above is also lawn
			if(*(*(Lawn+JUMP*j)+JUMP*(y-1)) != 0)
				{
				//Find right x-coordinate of new rectangle
				int newrec = j + 1;
				while(newrec < xsize && *(*(Lawn+JUMP*newrec)+JUMP*(y-1)) != 0)
					++newrec;
				--newrec;
				if(IsOnReclist(newrec, y-1, beg) == 0)
					{
					rectangles = InitNewRectangle(rectangles);
					if(rectangles == NULL)
						return 0;
					if(DownUpRectangle(Lawn, newrec, y-1, xsize, ysize, rectangles, beg) == 0)
						return 0;
					}
				//Check below new rectangle if still lawn
				while(j <= newrec)
					{
					if(*(*(Lawn+JUMP*j)+JUMP*y) == 0)
						j = xsize + 4;	//Found wall - break
					++j;				
					}
				//Counter upcoming ++j
				--j;
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
int CheckForUpDown2(char **Lawn, int x1, int x2, int y, int xsize, int ysize, reclist *rectangles, reclist *beg)
	{
	//Check for DownUps to the left of the rectangle
	int i = x1 - 1;
	if(*(*(Lawn+JUMP*x1)+JUMP*y) != 0)
		{
		//While row is still lawn
		while(i >= 0 && *(*(Lawn+JUMP*i)+JUMP*y) != 0)
			{
			//If pixel below is also lawn
			if(*(*(Lawn+JUMP*i)+JUMP*(y+1)) != 0)
				{
				//Find left x-coordinate of new rectangle
				int newrec = i - 1;
				while(newrec >= 0 && *(*(Lawn+JUMP*newrec)+JUMP*(y+1)) != 0)
					--newrec;
				++newrec;
				if(IsOnReclist(newrec, y+1, beg) == 0)
					{
					rectangles = InitNewRectangle(rectangles);
					if(rectangles == NULL)
						return 0;
					if(UpDownRectangle(Lawn, newrec, y+1, xsize, ysize, rectangles, beg) == 0)
						return 0;
					}
				//Check above new rectangle if still lawn
				while(i >= newrec)
					{
					if(*(*(Lawn+JUMP*i)+JUMP*y) == 0)
						i = -4;
					--i;				
					}
				//Counter upcoming --i
				++i;
				}
			--i;
			}
		}
	//Check for UpDowns to the right of the rectangle 
	int j = x2 + 1;
	if(*(*(Lawn+JUMP*x2)+JUMP*y) != 0)
		{
		//While row is still lawn
		while(j < xsize && *(*(Lawn+JUMP*j)+JUMP*y) != 0)
			{
			//If pixel below is also lawn
			if(*(*(Lawn+JUMP*j)+JUMP*(y+1)) != 0)
				{
				if(IsOnReclist(j, y+1, beg) == 0)
					{
					rectangles = InitNewRectangle(rectangles);
					if(rectangles == NULL)
						return 0;
					if(UpDownRectangle(Lawn, j, y+1, xsize, ysize, rectangles, beg) == 0)
						return 0;
					}
				//Check above new rectangle if still lawn
				++j;
				while(j < xsize && *(*(Lawn+JUMP*j)+JUMP*(y+1)) != 0)
					{
					if(*(*(Lawn+JUMP*j)+JUMP*y) == 0)
						i = xsize + 4;	//Found wall - break
					++j;
					}
				//Counter upcoming ++j
				--j;
				}
			++j;
			}
		}
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
int UpDownRectangle(char **Lawn, int x, int y, int xsize, int ysize, reclist *rectangles, reclist *beg)
	{
	rectangles -> x1 = x;	//FIX
	rectangles -> y1 = y;	//FIX
	int len = CalcLenRight(Lawn, x, y, xsize);
	int j = y + 1;
	//Iterate through proper rows
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
		if(CheckForUpDown(Lawn, x, j, xsize, ysize, len, rectangles, beg) == 0)
			return 0;
		if(CheckForDownUp(Lawn, x, x + len - 1, j, xsize, ysize, rectangles, beg) == 0)
			return 0;
		}
	return 1;
	}

/*
 * Analogical to UpDownRectangle but the
 * search is done from down-right corner 
 * to up-left corner
 */ 
int DownUpRectangle(char **Lawn, int x, int y, int xsize, int ysize, reclist *rectangles, reclist *beg)
	{
	rectangles -> x2 = x;	//FIX
	rectangles -> y2 = y;	//FIX
	int len = CalcLenLeft(Lawn, x, y);
	int j = y - 1;
	//Iterate through proper rows
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
		if(CheckForUpDown2(Lawn, x - len + 1, x, j, xsize, ysize, rectangles, beg) == 0)
			return 0;
		if(CheckForDownUp2(Lawn, x, j, xsize, ysize, len, rectangles, beg) == 0)
			return 0;
		}
	return 1;
	}

void PrintRectangles(reclist *r)
	{
	while(r != NULL)
		{
		printf("(%d, %d) - (%d, %d)\n", r->x1, r->y1, r->x2, r->y2);
		r = r -> next;
		}
	}

void MakeDecisions(char *mode, int *type, reclist *Rectangles, int radius)
	{
	int xlen = Rectangles->x2 - Rectangles->x1 + 1;
	int ylen = Rectangles->y2 - Rectangles->y1 + 1;
	if(xlen <= radius*2+1 && ylen <= radius*2+1)
		{
		*mode = 'm';
		return;
		}
	int curbest = xlen + ylen;
	for(int i = 3; i >= 0; --i)
		{
		if(i != 1)
			{
			if(xlen >= ((radius*(4-i))*2+1) && ylen >= ((radius*(4-i))*2+1) &&
			xlen%((radius*(4-i))*2+1) + ylen%((radius*(4-i))*2+1) <= curbest)
				{
				curbest = xlen%((radius*(4-i))*2+1) + ylen%((radius*(4-i))*2+1);
				*type = i;
				}
			}
		else
			{
			if(xlen >= ((radius*(4-i))*2+1) && ylen >= (radius*(4-i)+1) &&
			xlen%((radius*(4-i))*2+1) + ylen%(radius*(4-i)+1) <= curbest)
				{
				curbest = xlen%((radius*(4-i))*2+1) + ylen%(radius*(4-i)+1);
				*type = i;
				}
			}
		}
	}

int PlaceOneMiddle(char **Lawn, reclist *R, int time, /*sprlist *Sprinklers,*/ int Sprstats[], int type)
	{
	for(int i = 0; i < 3; i++)
		{
		/*if(PutSprinkler(Lawn, type*90+90, time, (R->x1+R->x2)/2, (R->y1+R->y2)/2, 0, Sprinklers) == 0)
			return 0;*/
		printf("x=%d    y=%d\n", (R->x1+R->x2)/2, (R->y1+R->y2)/2);
		}
	return 1;
	}

int PlaceOnYAxis(char **Lawn, reclist *R, int time, int radius, /*sprlist *Sprinklers,*/ int Sprstats[], int type)
	{
	int deg = 0;
	int shift = 1;
	int x;
	int jump;
	int stopparam;
	if(type == 0)
		x = R->x1;
	else
		x = (R->x1+R->x2)/2;
	if(type != 3)
		jump = 1 + radius;
	else
		jump = 1 + 2 * radius;
	if(type <= 1)
		stopparam = R->y2;
	else
		stopparam = R->y2-radius;
	int i = R->y1+radius;
	for(; i <= stopparam; i += jump)
		{
		if(type == 3)
			{
			for(int j = 0; j < 3; j++)
				{
				/*if(PutSprinkler(Lawn, 360, time, x, i, 0, Sprinklers) == 0)
					return 0;*/
				printf("x=%d    y=%d\n", x, i);
				}
			}
		else if(type == 2)
			{
			/*if(PutSprinkler(Lawn, 270, time, x, i, deg, Sprinklers) == 0)
					return 0;*/
			printf("x=%d    y=%d\n", x, i);
			if(deg == 0)
				deg = 180;
			else
				{
				deg = 0;
				i += radius;/////////////
				}
			x += shift;
			shift *= -1;
			}
		else
			{
			/*if(PutSprinkler(Lawn, 90+90*type, time, x, i, 0, Sprinklers) == 0)
					return 0;*/
			printf("x=%d    y=%d\n", x, i);
			}
		}
	if(deg == 180)
		{
		i -= jump;
		for(int k = 0; k < 3; k++)
			{
			/*if(PutSprinkler(Lawn, 360, time, x+radius/2+1, i+radius/2+1, 0, Sprinklers) == 0)
				return 0;*/
			printf("x=%d    y=%d\n", x+radius/2, i+radius/2+1);
			}
		}
	return 1;
	}

int PlaceOnXAxis(char **Lawn, reclist *R, int time, int radius, /*sprlist *Sprinklers,*/ int Sprstats[], int type)
	{
	int deg = 0;
	int shift = 1;
	int stopparam = R->x2 - radius;
	int i;
	int y;
	int jump;
	if(type == 0)
		i = R->x1;
	else
		i = R->x1 + radius;
	if(type <= 1)
		y = R->y2;
	else
		y = (R->y1+R->y2)/2;
	if(type%2 == 0)
		jump = radius+ 1 ;
	else
		jump = 2 * radius + 1;
	for(; i <= stopparam; i += jump)
		{
		if(type == 3)
			{
			for(int j = 0; j < 3; j++)
				{
				/*if(PutSprinkler(Lawn, 360, time, i, y, 0, Sprinklers) == 0)
					return 0;*/
				printf("x=%d    y=%d\n", i, y);
				}
			}
		else if(type == 2)
			{
			/*if(PutSprinkler(Lawn, 360, time, i, y, deg, Sprinklers) == 0)
					return 0;*/
			printf("x=%d    y=%d\n", i, y);
			if(deg == 0)
				deg = 180;
			else
				{
				deg = 0;
				i += radius;/////////////
				}
			y += shift;
			shift *= -1;
			}
		else
			{
			/*if(PutSprinkler(Lawn, 90+90*type, time, i, y, 0, Sprinklers) == 0)
				return 0;*/
			printf("x=%d    y=%d\n", i, y);
			}
		}
	if(deg == 180)
		{
		i -= jump;
		for(int k = 0; k < 3; k++)
			{
			/*if(PutSprinkler(Lawn, 360, time, i+radius/2+1, y+radius/2+1, 0, Sprinklers) == 0)
				return 0;*/
			printf("x=%d    y=%d\n", i+radius/2+1, y+radius/2);
			}
		}
	return 1;
	}

int PlaceSprRow(int y, char **Lawn, reclist *R, int time, int radius, /*sprlist *Sprinklers,*/ int Sprstats[], int type)
	{
	int stopparam = R->x2 - radius;
	int shift = 1;
	int deg = 0;
	int i;
	int jump;
	if(type == 0)
		i = R->x1;
	else
		i = R->x1 + radius;
	if(type%2 == 0)
		jump = radius + 1;
	else
		jump = 2 * radius + 1;
	for(; i <= stopparam; i += jump)
		{
		if(type == 3)
			{
			for(int j = 0; j < 3; j++)
				{
				/*if(PutSprinkler(Lawn, 360, time, i, y, 0, Sprinklers) == 0)
					return 0;*/
				printf("x=%d    y=%d\n", i, y);
				}
			}
		else if(type == 2)
			{
			/*if(PutSprinkler(Lawn, 360, time, i, y, deg, Sprinklers) == 0)
					return 0;*/
			printf("x=%d    y=%d\n", i, y);
			if(deg == 0)
				deg = 180;
			else
				{
				deg = 0;
				i += radius;/////////////
				}
			y += shift;
			shift *= -1;
			}
		else
			{
			/*if(PutSprinkler(Lawn, 90+90*type, time, i, y, 0, Sprinklers) == 0)
					return 0;*/
			printf("x=%d    y=%d\n", i, y);
			}
		}
	if(deg == 180)
		{
		i -= jump;
		for(int k = 0; k < 3; k++)
			{
			/*if(PutSprinkler(Lawn, 360, time, i+radius/2+1, y+radius/2+1, 0, Sprinklers) == 0)
					return 0;*/
			printf("x=%d    y=%d\n", i+radius/2+1, y+radius/2);
			}
		}
	return 1;
	}

int PlaceInRectangle(char **Lawn, reclist *R, int time, int radius, /*sprlist *Sprinklers,*/ int Sprstats[], int type)
	{
	int stopparam;
	int jump;
	if(type <= 1)
		{
		stopparam = R->y2;
		jump = radius + 1;
		}
	else
		{
		stopparam = R->y2 - radius;
		jump = 2 * radius + 1;
		}
	for(int i = R->y1 + radius; i <= stopparam; i += jump)
		{
		if(PlaceSprRow(i, Lawn, R, time, radius, /*Sprinklers,*/ Sprstats, type) == 0)
			return 0;
		}
	return 1;
	}

int PlaceSprGreedily(char **Lawn, reclist *R, int time, int radius, /*sprlist *Sprinklers,*/ int Sprstats[], int type)
	{
	int xlen = R->x2 - R->x1 + 1;
	int ylen = R->y2 - R->y1 + 1;
	if(xlen <= 2*radius+1)
		{
		return PlaceOnYAxis(Lawn, R, time, radius, /*Sprinklers,*/ Sprstats, type);
		}
	else if(ylen <= 2*radius+1)
		{
		return PlaceOnXAxis(Lawn, R, time, radius, /*Sprinklers,*/ Sprstats, type);
		}
	else
		{
		return PlaceInRectangle(Lawn, R, time, radius, /*Sprinklers,*/ Sprstats, type);
		}
	}

int FillRecGreedily(char **Lawn, reclist *Rectangles, int time, int radius, /*sprlist *Sprinklers,*/ int Sprstats[])
	{
	int errcode = 1;
	/*Sprinklers -> x = -1;
	Sprinklers -> next = NULL;*/
	char mode = 'h';	// m/h
	int type = 3;	// from 0 to 3		0-90 1-180 2-270 3-360
	while(Rectangles != NULL)
		{
		MakeDecisions(&mode, &type, Rectangles, radius);
		if(mode == 'm')
			errcode = PlaceOneMiddle(Lawn, Rectangles, time, /*Sprinklers,*/ Sprstats, 360);
		else	//if (mode == 'h')
			errcode = PlaceSprGreedily(Lawn, Rectangles, time, radius*(4-type), /*Sprinklers,*/ Sprstats, type);
		if(errcode == 0)
			return 0;
		Rectangles = Rectangles -> next;
		}
	return 1;
	}

int DoTheJob(char **Lawn, /*parameters *Param, sprlist *Sprinklers*/ int xsize, int ysize, int radius, int time)
	{
	int errcode = 1;
	pointlist *Areas = FindAreas(Lawn, xsize, ysize);
	pointlist *PivAreas = Areas;
	if(Areas == NULL)
		return 0;
	while(Areas != NULL)
		{
		reclist *Rectangles = malloc(sizeof(*Rectangles));
		if(Rectangles == NULL)
			{
			FreePoints(PivAreas);
			return 0;
			}
		Rectangles -> next = NULL;
		errcode = UpDownRectangle(Lawn, Areas->x, Areas->y, xsize, ysize, Rectangles, Rectangles);
		if(errcode == 0)
			{
			FreeRectangles(Rectangles);
			FreePoints(PivAreas);
			return 0;
			}
		int Sprstats[4] = {1, 2, 3, 4};
		errcode = FillRecGreedily(Lawn, Rectangles, time, radius, /*Sprinklers,*/ Sprstats);
		if(errcode == 0)
			{
			FreeRectangles(Rectangles);
			FreePoints(PivAreas);
			return 0;
			}
		FreeRectangles(Rectangles);
		Areas = Areas -> next;
		}
	//Param->currentmean = CalculateMean(Lawn, Param);
	/*
	Areas = PivAreas;
	while(Areas != NULL)
		{
		//(...)
		}
	*/
	FreePoints(PivAreas);
	return 1;	
	}
