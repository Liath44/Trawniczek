//stdlib, lawnholder, output
#include "gardener.h"

/*
	TODO:
 - "MERGE" SOME FUNCTIONS (PLACING SPRINKLERS IN FOR, OTHERS)
 - COMMENT & EDIT COMMENTS	
 - FUNCTION FOR SPRSTATS	
 - IS TURNING (TO MAKE CIRCLES OUT OF THEM) TYPES 1 and 0 BENEFICIAL
 - DO I UPDATE SPRINKLERS LIST OR MRBALLOG
 - FUNCTION CALCULATING MEAN ON AREA
 - BREAK SIGNATW INTO TWO FUNCTIONS - ONE CHECKS YMIN YMAX AND OTHER ONLY XMAX XMIN
 - ALTERNATIVE TO ATW->x = (ATW->xmin + ATW->xmax)/2;
 - WHEN PLACING IN ATW (ATW->xmin + ATW->xmax)/2 CAN BE WALL - THINK OF A BETTER THAN CURRENT SOLUTION
 - HALVING 180 AND 90 IS MORE TRICKY THAN I THOUGHT - THEY ARE AFTERALL NONSYMETRICAL - 180 _U_ scenario
 - PUTSPRINKLER SHOULD GET RADIUS360 AS PARAMETER
	TODO:
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
	pointlist *Outcome = malloc(sizeof(*Outcome));
	if(Outcome == NULL)
		return NULL;
	Outcome -> x = x;
	Outcome -> y = y;
	Outcome -> next = next;
	return Outcome;
	}

void FreePoints(pointlist *Point)
	{
	pointlist *Piv;
	while(Point != NULL)
		{
		Piv = Point -> next;
		free(Point);
		Point = Piv;
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
	pointlist *Ret = NULL;
	for(int j = 0; j < ysize; j++)
		{
		for(int i = 0; i < xsize; i++)
			{
			if(*(*(Lawn+JUMP*i)+JUMP*j) == 1)
				{
				//Found new area
				pointlist *Piv = NewPoint(i, j, Ret);
				if(Piv == NULL)
					{
					FreePoints(Ret);
					return NULL;
					}
				Ret = Piv;
				//Mark area by making it's pixels negative
				SignAreaJump(Lawn, xsize, ysize, i, j);
				}
			}
		}
	ResetSignsJump(Lawn, xsize, ysize);
	return Ret;
	}

void FreeRectangles(reclist *Rec)
	{
	reclist *Piv;
	while(Rec != NULL)
		{
		Piv = Rec -> next;
		free(Rec);
		Rec = Piv;
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
reclist *InitNewRectangle(reclist *Rectangles)
	{
	reclist *Piv = malloc(sizeof(*Piv));
	if(Piv == NULL)
		return NULL;
	while(Rectangles -> next != NULL)
		Rectangles = Rectangles -> next;
	Piv -> next = NULL;
	Rectangles -> next = Piv;
	return Piv;
	}

/*
 * Checks whether point (x, y) is inside one
 * of rectangles from reclist *Rl
 * 
 * Returns 1 if true
 * Returns 0 otherwise
 */
int IsOnReclist(int x, int y, reclist *Rl)
	{
	while(Rl != NULL)
		{
		if(x >= Rl->x1 && y >= Rl->y1 && x <= Rl->x2 && y <= Rl->y2)
			return 1;
		Rl = Rl -> next;
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
 * reclist *Rectangles - points to a certain part 
 * of list of already found rectangles
 * 
 * Returns 0 on memory allocation error
 * Returns 1 otherwise
 */
int CheckForUpDown(char **Lawn, int x, int j, int xsize, int ysize, int len, reclist *Rectangles, reclist *Beg)
	{
	//x coordinate of left-down corner
	int i1 = x + len - 1;
	//Check for UpDowns directly below rectangle
	while(i1 >= x)
		{
		//New rectangle when it's left wall is found
		if(*(*(Lawn+JUMP*i1)+JUMP*j) != 0 && (i1 == 0 || *(*(Lawn+JUMP*(i1-1))+JUMP*j) == 0))
			{
			if(IsOnReclist(i1*JUMP, j*JUMP, Beg) == 0)
				{
				Rectangles = InitNewRectangle(Rectangles);
				if(Rectangles == NULL)
					return 0;
				if(UpDownRectangle(Lawn, i1, j, xsize, ysize, Rectangles, Beg) == 0)
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
		if(IsOnReclist(i1*JUMP, j*JUMP, Beg) == 0)
			{
			Rectangles = InitNewRectangle(Rectangles);
			if(Rectangles == NULL)
				return 0;
			if(UpDownRectangle(Lawn, i1, j, xsize, ysize, Rectangles, Beg) == 0)
				return 0;
			}
		}
	return 1;
	}

/*
 * Analogical to CheckForDownUp but called
 * by DownUpRectangle
 */
int CheckForDownUp2(char **Lawn, int x, int j, int xsize, int ysize, int len, reclist *Rectangles, reclist *Beg)
	{
	//x coordinate of right-up corner
	int i1 = x - len + 1;
	//Check for DownUps directly above rectangle
	while(i1 <= x)
		{
		//New rectangle when it's right wall is found
		if(*(*(Lawn+JUMP*i1)+JUMP*j) != 0 && (i1 == xsize-1 || *(*(Lawn+JUMP*(i1+1))+JUMP*j) == 0))
			{
			if(IsOnReclist(i1*JUMP+JUMP-1, j*JUMP+JUMP-1, Beg) == 0)
				{
				Rectangles = InitNewRectangle(Rectangles);
				if(Rectangles == NULL)
					return 0;
				if(DownUpRectangle(Lawn, i1, j, xsize, ysize, Rectangles, Beg) == 0)
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
		if(IsOnReclist(i1*JUMP+JUMP-1, j*JUMP+JUMP-1, Beg) == 0)
			{
			Rectangles = InitNewRectangle(Rectangles);
			if(Rectangles == NULL)
				return 0;
			if(DownUpRectangle(Lawn, i1, j, xsize, ysize, Rectangles, Beg) == 0)
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
 * reclist *Rectangles - points to a certain part 
 * of list of already found rectangles
 * 
 * Returns 0 on memory allocation error
 * Returns 1 otherwise
 */
int CheckForDownUp(char **Lawn, int x1, int x2, int y, int xsize, int ysize, reclist *Rectangles, reclist *Beg)
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
				if(IsOnReclist(i*JUMP+JUMP-1, (y-1)*JUMP+JUMP-1, Beg) == 0)
					{
					Rectangles = InitNewRectangle(Rectangles);
					if(Rectangles == NULL)
						return 0;
					if(DownUpRectangle(Lawn, i, y-1, xsize, ysize, Rectangles, Beg) == 0)
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
				if(IsOnReclist(newrec*JUMP+JUMP-1, (y-1)*JUMP+JUMP-1, Beg) == 0)
					{
					Rectangles = InitNewRectangle(Rectangles);
					if(Rectangles == NULL)
						return 0;
					if(DownUpRectangle(Lawn, newrec, y-1, xsize, ysize, Rectangles, Beg) == 0)
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
int CheckForUpDown2(char **Lawn, int x1, int x2, int y, int xsize, int ysize, reclist *Rectangles, reclist *Beg)
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
				if(IsOnReclist(newrec*JUMP, (y+1)*JUMP, Beg) == 0)
					{
					Rectangles = InitNewRectangle(Rectangles);
					if(Rectangles == NULL)
						return 0;
					if(UpDownRectangle(Lawn, newrec, y+1, xsize, ysize, Rectangles, Beg) == 0)
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
				if(IsOnReclist(j*JUMP, (y+1)*JUMP, Beg) == 0)
					{
					Rectangles = InitNewRectangle(Rectangles);
					if(Rectangles == NULL)
						return 0;
					if(UpDownRectangle(Lawn, j, y+1, xsize, ysize, Rectangles, Beg) == 0)
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
 * reclist *Rectangles - stores to-be-found 
 * parameters of a rectangle
 * 
 * Returns 0 on memory allocation errors
 * Returns 1 otherwise
 */
int UpDownRectangle(char **Lawn, int x, int y, int xsize, int ysize, reclist *Rectangles, reclist *Beg)
	{
	Rectangles -> x1 = x * JUMP;
	Rectangles -> y1 = y * JUMP;
	int len = CalcLenRight(Lawn, x, y, xsize);
	int j = y + 1;
	//Iterate through proper rows
	int isend = CheckRow(Lawn, x, j, len, xsize, ysize);
	while(isend == 0)
		{
		j++;
		isend = CheckRow(Lawn, x, j, len, xsize, ysize);
		}
	Rectangles -> x2 = (x+len-1)*JUMP+JUMP-1;
	Rectangles -> y2 = (j-1)*JUMP+JUMP-1;
	if(j < ysize)
		{
		if(CheckForUpDown(Lawn, x, j, xsize, ysize, len, Rectangles, Beg) == 0)
			return 0;
		if(CheckForDownUp(Lawn, x, x + len - 1, j, xsize, ysize, Rectangles, Beg) == 0)
			return 0;
		}
	return 1;
	}

/*
 * Analogical to UpDownRectangle but the
 * search is done from down-right corner 
 * to up-left corner
 */ 
int DownUpRectangle(char **Lawn, int x, int y, int xsize, int ysize, reclist *Rectangles, reclist *Beg)
	{
	Rectangles -> x2 = x*JUMP+JUMP-1;
	Rectangles -> y2 = y*JUMP+JUMP-1;
	int len = CalcLenLeft(Lawn, x, y);
	int j = y - 1;
	//Iterate through proper rows
	int isend = CheckRow(Lawn, x - len + 1, j, len, xsize, ysize);
	while(isend == 0)
		{
		j--;
		isend = CheckRow(Lawn, x - len + 1, j, len, xsize, ysize);
		}
	Rectangles -> x1 = (x-len+1)*JUMP;
	Rectangles -> y1 = (j+1)*JUMP;
	if(j >= 0)
		{
		if(CheckForUpDown2(Lawn, x - len + 1, x, j, xsize, ysize, Rectangles, Beg) == 0)
			return 0;
		if(CheckForDownUp2(Lawn, x, j, xsize, ysize, len, Rectangles, Beg) == 0)
			return 0;
		}
	return 1;
	}

/*
 * Decides on how and with which sprinklers to water a Rectangle
 * 
 * char *mode - Decision on 'how' part of the subject problem
 * should be stored in variable pointed by mode
 * 
 * mode h - horizontal placement of sprinklers
 * mode m - place one sprinkler in the middle
 * 
 * int *type - Decision on 'with which' part of the subject 
 * problem should be stored in variable pointed by type
 * 
 * type n - place 90+(n*90) sprinklers where n = 0/1/2/3
 * 
 * reclist *Rectangles - Stores info on said rectangle
 * 
 * int radius - radius of 360 sprinkler
 */
void MakeDecisions(char *mode, int *type, reclist *Rectangles, int radius)
	{
	int xlen = Rectangles->x2 - Rectangles->x1 + 1;
	int ylen = Rectangles->y2 - Rectangles->y1 + 1;
	//Rectangle is too little for h mode
	if(xlen <= radius*2+1 && ylen <= radius*2+1)
		{
		*mode = 'm';
		return;
		}
	int curbest = xlen + ylen;
	//check how well all types fit
	for(int i = 3; i >= 0; --i)
		{
		if(i != 1)
			{
			//if sprinklers fit well in rectangle and do that better than previous types 
			if(xlen >= ((radius*(4-i))*2+1) && ylen >= ((radius*(4-i))*2+1) && 
			xlen%((radius*(4-i))*2+1) + ylen%((radius*(4-i))*2+1) <= curbest)
				{
				curbest = xlen%((radius*(4-i))*2+1) + ylen%((radius*(4-i))*2+1);
				*type = i;
				}
			}
		else
			{
			//alternative version for 180 type
			if(xlen >= ((radius*(4-i))*2+1) && ylen >= (radius*(4-i)+1) && 
			xlen%((radius*(4-i))*2+1) + ylen%(radius*(4-i)+1) <= curbest)
				{
				curbest = xlen%((radius*(4-i))*2+1) + ylen%(radius*(4-i)+1);
				*type = i;
				}
			}
		}
	}

/*
 * Places one sprinkler in the middle of a Rectangle
 * 
 * char **Lawn - stores the lawn
 * 
 * int xsize, ysize - lawn's size
 * 
 * reclist *R - stores info on rectangle
 * 
 * int time - how long lawn should be watered
 * 
 * sprlist *Sprinklers - list of already placed sprinklers
 * 
 * int type, radius - type and radius of sprinkler to be placed
 * In this function always type == 3
 * 
 * Returns 0 on PutSprinkler(...)'s failure
 * Returns 1 otherwise
 */
int PlaceOneMiddle(char **Lawn, int xsize, int ysize, reclist *R, int time, sprlist *Sprinklers, int type, int radius)
	{
	//Place 360 sprinkler 3 times
	for(int i = 0; i < 3; i++)
		{
		if(PutSprinkler(Lawn, xsize, ysize, type*90+90, radius, time, (R->x1+R->x2)/2, (R->y1+R->y2)/2, 0, Sprinklers) == 0)
			return 0;
		}
	return 1;
	}

/*
 * Places sprinklers horizontally in rectangle R
 *
 * char **Lawn - sotores the lawn
 * 
 * int xsize, ysize - lawn's size
 * 
 * reclist *R - parameters of said rectangle
 * 
 * int time - how long lawn should be watered
 * 
 * int type, radius - type and radius of sprinkler to be placed
 * In this function always type == 3
 *
 * sprlist *Sprinklers - list of already placed sprinklers
 * 
 * Returns 0 on PutSprinkler(...)'s failure
 * Returns 1 otherwise
 * 
 * Functions' other parameters:
 * 
 * int x - column to place sprinklers
 * 
 * int jump - number of pixels between y-coords of middles 
 * of next two sprinklers
 * 
 * int i - row in which next sprinkler should be placed
 * 
 * int stopparam - don't place sprinklers after this y-coord
 * 
 * int deg - 270 sprinklers are placed alternately (0 - 180)
 * deg stores current degree to be used
 * 
 * int shift - when placing 270 sprinklers they should be
 * placed in x and x+1 column alternately
 */
int PlaceOnYAxis(char **Lawn, int xsize, int ysize, reclist *R, int time, int radius, sprlist *Sprinklers, int type)
	{
	//set proper parameters
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
	stopparam = R->y2;
	int i = R->y1+radius;
	jump /= 2;
	//place sprinklers in column
	for(; i <= stopparam; i += jump)
		{
		if(type == 3)
			{
			//place 3 times for 360 sprinkler
			for(int j = 0; j < 3; j++)
				{
				if(PutSprinkler(Lawn, xsize, ysize, 360, radius, time, x, i, 0, Sprinklers) == 0)
					return 0;
				}
			}
		else if(type == 2)
			{
			if(PutSprinkler(Lawn, xsize, ysize, 270, radius, time, x, i, deg, Sprinklers) == 0)
					return 0;
			//turn next sprinkler to fit currently placed sprinkler
			if(deg == 0)
				deg = 180;
			//new formation - bigger i update
			else
				{
				deg = 0;
				i += radius;
				}
			//change to x/x+1 column and prepare for next shift
			x += shift;
			shift *= -1;
			}
		else//if type == 1 || type == 0
			{
			if(PutSprinkler(Lawn, xsize, ysize, 90+90*type, radius, time, x, i, 0, Sprinklers) == 0)
					return 0;
			}
		}
	//if number of 270 sprinklers is odd additional 360 can be placed easily
	//deg == 180 only if type == 2
	if(deg == 180)
		{
		i -= jump;
		for(int k = 0; k < 3; k++)
			{
			if(PutSprinkler(Lawn, xsize, ysize, 360, radius, time, x+radius/2, i+radius/2+1, 0, Sprinklers) == 0)
				return 0;
			}
		}
	return 1;
	}

/*
 * Analogical to PlaceOnYAxis but sprinklers are placed 
 * vertically in rectangle R
 */
int PlaceOnXAxis(char **Lawn, int xsize, int ysize, reclist *R, int time, int radius, sprlist *Sprinklers, int type)
	{
	//set proper parameters
	int deg = 0;
	int shift = 1;
	int stopparam = R->x2;
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
	jump /= 2;
	//place sprinklers in row
	for(; i <= stopparam; i += jump)
		{
		if(type == 3)
			{
			//place 3 times for 360 sprinkler
			for(int j = 0; j < 3; j++)
				{
				if(PutSprinkler(Lawn, xsize, ysize, 360, radius, time, i, y, 0, Sprinklers) == 0)
					return 0;
				}
			}
		else if(type == 2)
			{
			if(PutSprinkler(Lawn, xsize, ysize, 360, radius, time, i, y, deg, Sprinklers) == 0)
					return 0;
			//turn next sprinkler to fit currently placed sprinkler
			if(deg == 0)
				deg = 180;
			//new formation - bigger i update
			else
				{
				deg = 0;
				i += radius;
				}
			//change to y/y+1 row and prepare for next shift
			y += shift;
			shift *= -1;
			}
		else//if type == 1 || type == 0
			{
			if(PutSprinkler(Lawn, xsize, ysize, 90+90*type, radius, time, i, y, 0, Sprinklers) == 0)
				return 0;
			}
		}
	//if number of 270 sprinklers is odd additional 360 can be placed easily
	//deg == 180 only if type == 2
	if(deg == 180)
		{
		i -= jump;
		for(int k = 0; k < 3; k++)
			{
			if(PutSprinkler(Lawn, xsize, ysize, 360, radius, time, i+radius/2+1, y+radius/2, 0, Sprinklers) == 0)
				return 0;
			}
		}
	return 1;
	}

/*
 * Analogical to PlaceOnYAxis but row is specified by 
 * int y parameter
 */
int PlaceSprRow(int y, int xsize, int ysize, char **Lawn, reclist *R, int time, int radius, sprlist *Sprinklers, int type)
	{
	//set proper parameters
	int stopparam = R->x2;
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
	jump /= 2;
	//place sprinklers in row
	for(; i <= stopparam; i += jump)
		{
		if(type == 3)
			{
			//place 3 times for 360 sprinkler
			for(int j = 0; j < 3; j++)
				{
				if(PutSprinkler(Lawn, xsize, ysize, 360, radius, time, i, y, 0, Sprinklers) == 0)
					return 0;
				}
			}
		else if(type == 2)
			{
			if(PutSprinkler(Lawn, xsize, ysize, 270, radius, time, i, y, deg, Sprinklers) == 0)
					return 0;
			//turn next sprinkler to fit currently placed sprinkler
			if(deg == 0)
				deg = 180;
			//new formation - bigger i update
			else
				{
				deg = 0;
				i += radius;
				}
			//change to y/y+1 row and prepare for next shift
			y += shift;
			shift *= -1;
			}
		else//if type == 1 || type == 0
			{
			if(PutSprinkler(Lawn, xsize, ysize, 90+90*type, radius, time, i, y, 0, Sprinklers) == 0)
					return 0;
			}
		}
	//if number of 270 sprinklers is odd additional 360 can be placed easily
	//deg == 180 only if type == 2
	if(deg == 180)
		{
		i -= jump;
		for(int k = 0; k < 3; k++)
			{
			if(PutSprinkler(Lawn, xsize, ysize, 360, radius, time, i+radius/2+1, y+radius/2, 0, Sprinklers) == 0)
					return 0;
			}
		}
	return 1;
	}
/*
 * Places sprinklers in n rows. n is specified
 * based on rectangle R parameters
 * 
 * Parameters and return cases are analogical to 
 * PlaceOnyAxis(...)
 */
int PlaceInRectangle(char **Lawn, int xsize, int ysize, reclist *R, int time, int radius, sprlist *Sprinklers, int type)
	{
	//set proper parameters
	int stopparam;
	int jump;
	if(type <= 1)
		{
		stopparam = R->y2;
		jump = radius + 1;
		}
	else
		{
		stopparam = R->y2;
		jump = 2 * radius + 1;
		}
	jump /= 2;
	//place sprinklers in rows
	for(int i = R->y1 + radius; i <= stopparam; i += jump)
		{
		if(PlaceSprRow(i, xsize, ysize, Lawn, R, time, radius, Sprinklers, type) == 0)
			return 0;
		}
	return 1;
	}

/*
 * Places sprinklers in rectangle R according
 * to its size
 * 
 * Parameters descriptions are analogical to
 * PlaceOnYAxis(...)
 * 
 * Returns 0 on failure
 * Returns 1 otherwise
 */
int PlaceSprGreedily(char **Lawn, int xsize, int ysize, reclist *R, int time, int radius, sprlist *Sprinklers, int type)
	{
	int xlen = R->x2 - R->x1 + 1;
	int ylen = R->y2 - R->y1 + 1;
	if(xlen <= 2*radius+1)
		{
		return PlaceOnYAxis(Lawn, xsize, ysize, R, time, radius, Sprinklers, type);
		}
	else if(ylen <= 2*radius+1)
		{
		return PlaceOnXAxis(Lawn, xsize, ysize, R, time, radius, Sprinklers, type);
		}
	else
		{
		return PlaceInRectangle(Lawn, xsize, ysize, R, time, radius, Sprinklers, type);
		}
	}

/*
 * Fills every rectangle from Rectangles with sprinklers
 * which types are chosen based on said rectangle parameters
 * 
 * char **Lawn - stores the lawn
 * 
 * int xsize, ysize - lawn's size
 * 
 * reclist *Rectangles - list of to be filled rectangles
 * 
 * int time - time spent on lawn's watering
 * 
 * int radius - radius of 360 sprinkler
 * 
 * sprlist *Sprinklers - list of already placed sprinklers
 * that should be updated by newly placed sprinklers
 * 
 * Returns 0 on failure
 * Returns 1 otherwise
 * 
 * Functions' other parameters:
 * 
 * char mode - rectangles can be filled either horizontally/
 * vertically (mode h) or there can be just one sprinkler
 * in the middle (mode m) - mode is updated in MakeDecisions(...)
 * 
 * int type - rectangles can be filled with different types
 * of sprinklers:
 * 0 - 90 degree sprinklers
 * 1 - 180 degree sprinklers 
 * 2 - 270 degree sprinklers 
 * 3 - 360 degree sprinklers 
 * type is updated in MakeDecisions(...) 
 */
int FillRecGreedily(char **Lawn, int xsize, int ysize, reclist *Rectangles, int time, int radius, sprlist *Sprinklers)
	{
	int errcode = 1;
	//first sprinkler is a decoy
	Sprinklers -> x = -1;
	Sprinklers -> next = NULL;
	char mode = 'h';
	int type = 3;	
	//iterate through rectangles
	while(Rectangles != NULL)
		{
		//decide which mode and type to use
		MakeDecisions(&mode, &type, Rectangles, radius);
		if(mode == 'm')
			errcode = PlaceOneMiddle(Lawn, xsize, ysize, Rectangles, time, Sprinklers, 360, radius);
		else	//if (mode == 'h')
			errcode = PlaceSprGreedily(Lawn, xsize, ysize, Rectangles, time, radius*(4-type), Sprinklers, type);
		if(errcode == 0)
			return 0;
		Rectangles = Rectangles -> next;
		}
	return 1;
	}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*

void InitATW(areatowater *ATW, int radius, int xsize, int ysize)
	{
	ATW -> x = -1;
	ATW -> y = -1;
	ATW -> pixcount = 0;
	ATW -> xmax = -1;
	ATW -> xmin = xsize;
	ATW -> ymax = -1;
	ATW -> ymin = ysize;
	ATW -> besttype = 3;
	ATW -> bestdeg = 0;
	ATW -> bestscore = 4*radius+xsize+4*radius+ysize;
	}

void UpdateMinMax(areatowater *ATW, int x, int y)
	{
	if(x < ATW->xmin)
		ATW->xmin = x;
	else if(x > ATW->xmax)
		ATW->xmax = x;
	if(y < ATW->ymin)
		ATW->ymin = y;
	else if(y > ATW->ymax)
		ATW->ymax = y;
	}

void TryToFit360(char **Lawn, areatowater *ATW, int radius)
	{
	int score = abs(ATW->xmax - ATW->xmin - 2*radius) + abs(ATW->ymax - ATW->ymin - 2*radius);
	int xc = (ATW->xmin + ATW->xmax)/2;
	int yc = (ATW->ymin + ATW->ymax)/2;
	if(score < ATW->bestscore && *(*(Lawn+xc)+yc) != 0)
		{
		ATW->x = xc;
		ATW->y = yc;
		ATW->besttype = 3;
		ATW->bestdeg = 0;
		ATW->bestscore = score;
		}
	}


int CountSignedPix(char **Lawn, int xl, int xr, int yu, int yd)
	{
	int count = 0;
	for(int j = yu; j <= yd; j++)
		{
		for(int i = xl; i <= xl; i++)
			{
			if(*(*(Lawn+i)+j) < 0)
				++count;
			}
		}
	return count;
	}

void TryToFit180(char **Lawn, areatowater *ATW, int radius)
	{
	int score = abs(ATW->ymax - ATW->ymin - radius) + abs(ATW->xmax - ATW->xmin - 2*radius);
	int score2 = abs(ATW->ymax - ATW->ymin - 2*radius) + abs(ATW->xmax - ATW->xmin - radius);
	int xc = (ATW->xmin + ATW->xmax)/2;
	int yc = (ATW->ymin + ATW->ymax)/2;
	if(score < ATW->bestscore && score < score2)
		{
		int pixcount1 = CountSignedPix(Lawn, ATW->xmin, ATW->xmax, ATW->ymin, (ATW->ymax + ATW->ymin)/2);
		int pixcount2 = CountSignedPix(Lawn, ATW->xmin, ATW->xmax, (ATW->ymax + ATW->ymin)/2, ATW->ymax);
		if(pixcount1 < pixcount2 && *(*(Lawn+xc)+ATW->ymax) != 0)
			{
			ATW->x = xc;
			ATW->besttype = 1;
			ATW->bestscore = score;
			ATW->y = ATW->ymax;
			ATW->bestdeg = 0;
			}
		else if(*(*(Lawn+xc)+ATW->ymin) != 0)//U
			{
			ATW->x = (ATW->xmin + ATW->xmax)/2;
			ATW->besttype = 1;
			ATW->bestscore = score;
			ATW->y = ATW->ymin;
			ATW->bestdeg = 180;
			}
		}
	else if(score2 < ATW->bestscore)
		{
		int pixcount1 = CountSignedPix(Lawn, ATW->xmin, (ATW->xmin + ATW->xmax)/2, ATW->ymin, ATW->ymax);
		int pixcount2 = CountSignedPix(Lawn, (ATW->xmin + ATW->xmax)/2, ATW->xmax, ATW->ymin, ATW->ymax);
		if(pixcount1 < pixcount2 && *(*(Lawn+ATW->xmin)+yc) != 0)
			{
			ATW->y = yc;
			ATW->besttype = 1;
			ATW->bestscore = score2;
			ATW->x = ATW->xmin;
			ATW->bestdeg = 270;
			}
		else if(*(*(Lawn+ATW->xmax)+yc) != 0)//c
			{
			ATW->y = yc;
			ATW->besttype = 1;
			ATW->bestscore = score2;
			ATW->x = ATW->xmax;
			ATW->bestdeg = 90;
			}
		}
	}

void TryToFit270(char **Lawn, areatowater *ATW, int radius)//TODO: FIX TO ADD 360 - no, add that during placing sprinklers
	{
	int score = abs(ATW->xmax - ATW->xmin - 2*radius) + abs(ATW->ymax - ATW->ymin - 2*radius);
	int xc = (ATW->xmin + ATW->xmax)/2;
	int yc = (ATW->ymin + ATW->ymax)/2;
	if(score < ATW->bestscore && *(*(Lawn+xc)+yc) != 0)
		{
		ATW->x = xc;
		ATW->y = yc;
		ATW->besttype = 2;
		ATW->bestdeg = 0;
		ATW->bestscore = score;
		}
	}

void SortTab(int tab[], int size)
	{
	for(int i = 1; i < size; i++)
		{
		int j = i;
		while(j != 0 && tab[j] < tab[j-1])
			{
			int piv = tab[j];
			tab[j] = tab[j-1];
			tab[j-1] = piv;
			--j;
			}
		}
	}

void TryToFit90(char **Lawn, areatowater *ATW, int radius)
	{
	int score = abs(ATW->xmax - ATW->xmin - radius) + abs(ATW->ymax - ATW->ymin - radius);
	int xc = (ATW->xmin + ATW->xmax)/2;
	int yc = (ATW->ymin + ATW->ymax)/2;
	if(score < ATW->bestscore)
		{
		int pixcounttab[] =
			{
			CountSignedPix(Lawn, ATW->xmin, xc, ATW->ymin, yc),
			CountSignedPix(Lawn, xc, ATW->xmax, ATW->ymin, yc),
			CountSignedPix(Lawn, ATW->xmin, xc, yc, ATW->ymax),
			CountSignedPix(Lawn, xc, ATW->xmax, yc, ATW->ymax)
			};
		int sortedtab[] =
			{
			pixcounttab[0],
			pixcounttab[1],
			pixcounttab[2],
			pixcounttab[3]
			};
		SortTab(sortedtab, 4);
		int cont = 1;
		for(int i = 0; i < 4 && cont == 1; i++)
			{
			if(sortedtab[i] == pixcounttab[0] && *(*(Lawn+ATW->xmax)+ATW->ymax) != 0)
				{
				cont = 0;
				ATW->x = ATW->xmax;
				ATW->y = ATW->ymax;
				ATW->besttype = 0;
				ATW->bestdeg = 90;
				ATW->bestscore = score;
				}
			else if(sortedtab[i] == pixcounttab[1] && *(*(Lawn+ATW->xmin)+ATW->ymax) != 0)
				{
				cont = 0;
				ATW->x = ATW->xmin;
				ATW->y = ATW->ymax;
				ATW->besttype = 0;
				ATW->bestdeg = 0;
				ATW->bestscore = score;
				}
			else if(sortedtab[i] == pixcounttab[2] && *(*(Lawn+ATW->xmax)+ATW->ymin) != 0)
				{
				cont = 0;
				ATW->x = ATW->xmax;
				ATW->y = ATW->ymin;
				ATW->besttype = 0;
				ATW->bestdeg = 180;
				ATW->bestscore = score;
				}
			else if(sortedtab[i] == pixcounttab[3] && *(*(Lawn+ATW->xmin)+ATW->ymin) != 0)
				{
				cont = 0;
				ATW->x = ATW->xmin;
				ATW->y = ATW->ymin;
				ATW->besttype = 0;
				ATW->bestdeg = 270;
				ATW->bestscore = score;
				}
			}
		}
	}

void TryToUpdateATW(char **Lawn, areatowater *ATW, char *cont, int xsize, int ysize, int radius360, int Sprstats[])
	{
	if((int)Sprstats[3]*STATERR == ATW->pixcount)
		TryToFit360(Lawn, ATW, radius360);
	//if((int)Sprstats[3]*STATERR/2 == ATW->pixcount)
		//TryToFitHalf360(Lawn, ATW, xsize, ysize, radius360);
	if((int)Sprstats[2]*STATERR == ATW->pixcount)
		TryToFit270(Lawn, ATW, 2*radius360);
	//if((int)Sprstats[2]*STATERR/2 == ATW->pixcount)
		//TryToFitHalf270(Lawn, ATW, xsize, ysize, 2*radius360);
	if((int)Sprstats[1]*STATERR == ATW->pixcount)
		TryToFit180(Lawn, ATW, 3*radius360);
	//if((int)Sprstats[1]*STATERR/2 == ATW->pixcount)
		//TryToFitHalf180(Lawn, ATW, xsize, ysize, 3*radius360);
	if((int)Sprstats[0]*STATERR == ATW->pixcount)
		TryToFit90(Lawn, ATW, 4*radius360);
	//if((int)Sprstats[0]*STATERR/2 == ATW->pixcount)
		//TryToFitHalf90(Lawn, ATW, xsize, ysize, 4*radius360);
	if((int)Sprstats[1]*STATERR < ATW->pixcount)
		*cont = 'n';
	}

void SignATW(char **Lawn, int x, int y, areatowater *ATW, char *cont, int xsize, int ysize, double currmean, int radius360, int Sprstats[])
	{
	if(*cont == 'n')
		return;
	ATW->pixcount += 1;
	UpdateMinMax(ATW, x, y);
	*(*(Lawn+x)+y) *= -1;
	TryToUpdateATW(Lawn, ATW, cont, xsize, ysize, radius360, Sprstats);
	if(ATW->pixcount % 4 == 0)
		{
		if(x+1 < xsize && *(*(Lawn+x+1)+y) <= currmean && *(*(Lawn+x+1)+y) > 0)
			SignATW(Lawn, x+1, y, ATW, cont, xsize, ysize, currmean, radius360, Sprstats);
		if(y-1 >= 0 && *(*(Lawn+x)+y-1) <= currmean && *(*(Lawn+x)+y-1) > 0)
			SignATW(Lawn, x, y-1, ATW, cont, xsize, ysize, currmean, radius360, Sprstats);
		if(x-1 >= 0 && *(*(Lawn+x-1)+y) <= currmean && *(*(Lawn+x-1)+y) > 0)
			SignATW(Lawn, x-1, y, ATW, cont, xsize, ysize, currmean, radius360, Sprstats);
		if(y+1 < ysize && *(*(Lawn+x)+y+1) <= currmean && *(*(Lawn+x)+y+1) > 0)
			SignATW(Lawn, x, y+1, ATW, cont, xsize, ysize, currmean, radius360, Sprstats);
		}
	else if(ATW->pixcount % 4 == 1)
		{
		if(y+1 < ysize && *(*(Lawn+x)+y+1) <= currmean && *(*(Lawn+x)+y+1) > 0)
			SignATW(Lawn, x, y+1, ATW, cont, xsize, ysize, currmean, radius360, Sprstats);
		if(x+1 < xsize && *(*(Lawn+x+1)+y) <= currmean && *(*(Lawn+x+1)+y) > 0)
			SignATW(Lawn, x+1, y, ATW, cont, xsize, ysize, currmean, radius360, Sprstats);
		if(y-1 >= 0 && *(*(Lawn+x)+y-1) <= currmean && *(*(Lawn+x)+y-1) > 0)
			SignATW(Lawn, x, y-1, ATW, cont, xsize, ysize, currmean, radius360, Sprstats);
		if(x-1 >= 0 && *(*(Lawn+x-1)+y) <= currmean && *(*(Lawn+x-1)+y) > 0)
			SignATW(Lawn, x-1, y, ATW, cont, xsize, ysize, currmean, radius360, Sprstats);
		}
	else if(ATW->pixcount % 4 == 2)
		{
		if(x-1 >= 0 && *(*(Lawn+x-1)+y) <= currmean && *(*(Lawn+x-1)+y) > 0)
			SignATW(Lawn, x-1, y, ATW, cont, xsize, ysize, currmean, radius360, Sprstats);
		if(y+1 < ysize && *(*(Lawn+x)+y+1) <= currmean && *(*(Lawn+x)+y+1) > 0)
			SignATW(Lawn, x, y+1, ATW, cont, xsize, ysize, currmean, radius360, Sprstats);
		if(x+1 < xsize && *(*(Lawn+x+1)+y) <= currmean && *(*(Lawn+x+1)+y) > 0)
			SignATW(Lawn, x+1, y, ATW, cont, xsize, ysize, currmean, radius360, Sprstats);
		if(y-1 >= 0 && *(*(Lawn+x)+y-1) <= currmean && *(*(Lawn+x)+y-1) > 0)
			SignATW(Lawn, x, y-1, ATW, cont, xsize, ysize, currmean, radius360, Sprstats);
		}
		else	//if(ATW->pixcount % 4 == 3)
		{
		if(y-1 >= 0 && *(*(Lawn+x)+y-1) <= currmean && *(*(Lawn+x)+y-1) > 0)
			SignATW(Lawn, x, y-1, ATW, cont, xsize, ysize, currmean, radius360, Sprstats);
		if(x-1 >= 0 && *(*(Lawn+x-1)+y) <= currmean && *(*(Lawn+x-1)+y) > 0)
			SignATW(Lawn, x-1, y, ATW, cont, xsize, ysize, currmean, radius360, Sprstats);
		if(y+1 < ysize && *(*(Lawn+x)+y+1) <= currmean && *(*(Lawn+x)+y+1) > 0)
			SignATW(Lawn, x, y+1, ATW, cont, xsize, ysize, currmean, radius360, Sprstats);
		if(x+1 < xsize && *(*(Lawn+x+1)+y) <= currmean && *(*(Lawn+x+1)+y) > 0)
			SignATW(Lawn, x+1, y, ATW, cont, xsize, ysize, currmean, radius360, Sprstats);
		}
	}

void UnsignATW(char **Lawn, int x, int y, int xsize, int ysize)
	{
	*(*(Lawn+x)+y) *= -1;
	if(x+1 < xsize && *(*(Lawn+x+1)+y) < 0)
		UnsignATW(Lawn, x+1, y, xsize, ysize);
	if(x-1 >= 0 && *(*(Lawn+x-1)+y) < 0)
		UnsignATW(Lawn, x-1, y, xsize, ysize);
	if(y+1 < ysize && *(*(Lawn+x)+y+1) < 0)
		UnsignATW(Lawn, x, y+1, xsize, ysize);
	if(y-1 >= 0 && *(*(Lawn+x)+y-1) < 0)
		UnsignATW(Lawn, x, y-1, xsize, ysize);
	}

int CountAbsoluteSum(char **Lawn, int xl, int xr, int yu, int yd)
	{
	int count = 0;
	for(int j = yu; j <= yd; j++)
		{
		for(int i = xl; i <= xl; i++)
			{
			count += *(*(Lawn+i)+j);
			}
		}
	return count;
	}

//PutSprinkler(char **Lawn, int xsize, int ysize, int type, int radius, int time, int x, int y, int deg, sprlist *Sprinklers);
int PlaceSprinklersInATW(char **Lawn, areatowater *ATW, parameters *Param, int Sprstats[], int *nospr, sprlist *Sprinklers)
	{
	double signedno = (ATW->xmax - ATW->xmin + 1)*(ATW->ymax - ATW->ymin + 1);
	double premean = (double)CountAbsoluteSum(Lawn, ATW->xmin, ATW->xmax, ATW->ymin, ATW->ymax)/signedno;
	if(PutSprinkler(Lawn, Param->xsize, Param->ysize, 90+(ATW->besttype)*90, (Param->radius360)*(4-ATW->besttype), Param->time, ATW->x, ATW->y, ATW->bestdeg, Sprinklers) == 0)
		return 0;
	++(*nospr);
	double aftermean = (double)CountAbsoluteSum(Lawn, ATW->xmin, ATW->xmax, ATW->ymin, ATW->ymax)/signedno;
	double meandiff = aftermean - premean;
	while(aftermean < Param->currentmean)
		{
		if(PutSprinkler(Lawn, Param->xsize, Param->ysize, 90+(ATW->besttype)*90, (Param->radius360)*(4-ATW->besttype), Param->time, ATW->x, ATW->y, ATW->bestdeg, Sprinklers) == 0)
			return 0;
		++(*nospr);
		aftermean += meandiff;
		}
	return 1;
	}
//CountSignedPix(char **Lawn, int xl, int xr, int yu, int yd)
int TryToPlaceMore(char **Lawn, parameters *Param, reclist *Rectangles, int Sprstats[], sprlist *Sprinklers)
	{
	int nospr = 0;
	int sprwasadded = 1;
	while(sprwasadded == 1 && nospr <= MAXSPR)
		{
		sprwasadded = 0;
		reclist *Piv = Rectangles;
		while(Piv != NULL)
			{
			for(int j = Piv->y1; j <= Piv->y2; j++)
				{
				for(int i = Piv->x1; i <= Piv->x2; j++)
					{
					if(*(*(Lawn+i)+j) <= Param->currentmean)
						{
						areatowater ATW;
						int xstart = i;
						int ystart = j;
						char cont = 'y';
						InitATW(&ATW, Param->radius360, Param->xsize, Param->ysize);
						SignATW(Lawn, i, j, &ATW, &cont, Param->xsize, Param->ysize, Param->currentmean, Param->radius360, Sprstats);
						UnsignATW(Lawn, xstart, ystart, Param->xsize, Param->ysize);
						if(ATW.x != -1)
							{
							sprwasadded = 1;
							if(PlaceSprinklersInATW(Lawn, &ATW, Param, Sprstats, &nospr, Sprinklers) == 0)
								return 0;
							Param->currentmean = CalculateMean(Lawn, Param);
							}
						else
							{
							sprwasadded = 0;
							}
						i = ATW.xmax;//not the best thing to do but might be necessary
						}
					}
				}
			Piv = Piv -> next;
			}
		Param->currentmean = CalculateMean(Lawn, Param);
		}
	return 1;
	}

int PixCoveredQuadrant(int radius)
	{
	int outcome = 0;
	for(int i = 1; i <= radius; ++i)
		{
		for(int j = 1; j <= radius; ++j)
			{
			if(i*i + j*j <= radius*radius)
				++outcome;
			}
		}
	return outcome;
	}

int PixCovered90(int radius)
	{
	return 1 + 2*radius + PixCoveredQuadrant(radius);
	}

int PixCovered180(int radius)
	{
	return 1 + 3*radius + 2*PixCoveredQuadrant(radius);
	}

int PixCovered270(int radius)
	{
	return 1 + 4*radius + 3*PixCoveredQuadrant(radius);
	}

int PixCovered360(int radius)
	{
	return 1 + 4*radius + 4*PixCoveredQuadrant(radius);
	}

*/

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int DoTheJob(char **Lawn, parameters *Param, sprlist *Sprinklers)
	{
	int errcode = 1;
	//divide Lawn into separate areas
	pointlist *Areas = FindAreas(Lawn, Param->xsize/JUMP, Param->ysize/JUMP);
	pointlist *PivAreas = Areas;
	if(Areas == NULL)
		return 0;
	//int Sprstats[4] = {0, 0, 0, 0 /* TODO: DODAĆ FUNKCJE OBLICZAJĄCE */};
	//iterate through found areas
	while(Areas != NULL)
		{
		reclist *Rectangles = malloc(sizeof(*Rectangles));
		if(Rectangles == NULL)
			{
			FreePoints(PivAreas);
			return 0;
			}
		Rectangles -> next = NULL;
		//divide Area into rectangles
		errcode = UpDownRectangle(Lawn, Areas->x, Areas->y, Param->xsize/JUMP, Param->ysize/JUMP, Rectangles, Rectangles);
		if(errcode == 0)
			{
			FreeRectangles(Rectangles);
			FreePoints(PivAreas);
			return 0;
			}
		//fill rectangles in area with sprinklers
		errcode = FillRecGreedily(Lawn, Param->xsize, Param->ysize, Rectangles, Param->time, Param->radius360, Sprinklers);
		if(errcode == 0)
			{
			FreeRectangles(Rectangles);
			FreePoints(PivAreas);
			return 0;
			}
		//update mean
		Param->currentmean = CalculateMean(Lawn, Param);
		/*
		if(TryToPlaceMore(Lawn, Param, Rectangles, Sprstats, Sprinklers) == 0);
			{
			FreeRectangles(Rectangles);
			FreePoints(PivAreas);
			return 0;
			}*/
		FreeRectangles(Rectangles);
		Areas = Areas -> next;
		}
	FreePoints(PivAreas);
	return 1;	
	}
