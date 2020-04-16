#ifndef LAWNHOLDER
#define LAWNHOLDER

#include <stdlib.h>

/*
 * Structure for holding lawn's parameters
 */
struct _parameters
	{
	int xsize;
	int ysize;
	int pixelsize; //size of one character in the array
	int nlawn;	//Number of waterable pixels
	int time;	//time spent on watering
	int radius360;	//radius of 360 sprinkler
	double currentmean;
	};
typedef struct _parameters parameters;

/*
 * List of sprinklers
 */
typedef struct _sprlist
	{
	int type;	//90/180/270/360
	int x;
	int y;
	struct _sprlist *next;
	} sprlist;

/*
 * Puts subject sprinkler on list
 *
 * sprlist *Sprinklers - list of already placed sprinklers
 *
 * int type - sprinkler's type (90/180/270/360)
 *
 * int x, y - sprinkler's coordinates
 *
 * Returns 0 on failure
 */
int PutOnList(sprlist *Sprinklers, int type, int x, int y);

/*
 * Deallocates list of sprinklers
 * 
 * sprlist *Sprinklers - first node of sprinklers' list
 */
void FreeSprinklers(sprlist *Sprinklers);

/*
 * Calculates arithmetic mean of pixel's waterlvl in Lawn
 *
 * char **Lawn - matrix that stores lawn's pixels
 *
 * parameters *Param - points to structure that holds
 * lawn's parameters
 *
 * Function does not update currentmean field in parameters
 * structure. Returns said mean.
 */
double CalculateMean(char **Lawn, parameters *Param);

/*
 * Places sprinkler and waters the lawn
 *
 * char **Lawn - matrix that stores lawn's pixels
 * 
 * int xsize, ysize - Lawn's size
 *
 * int type - sprinkler's type (90/180/270/360)
 *
 * int time - duration of watering
 *
 * int radius - sprinkler's radius
 *
 * int x, y - sprinkler's coordinates
 *
 * int deg - degree between OX and circular's segment "arm" -
 * the one that is nearer OX when circular segment is placed
 * with rounded side upwards
 *
 * sprlist *Sprinklers - list of already placed sprinklers
 *
 * Returns 0 on failure
 */
int PutSprinkler(char **Lawn, int xsize, int ysize, int type, int radius, int time, int x, int y, int deg, sprlist *Sprinklers);

#endif
