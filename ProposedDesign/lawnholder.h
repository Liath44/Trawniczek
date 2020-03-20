#ifndef LAWNHOLDER
#define LAWNHOLDER

#include <stdlib.h>
#include "output.h"

/*
 * Structure for holding a lawn
 */
struct _lawn
	{
	short x;//x & y parameters might be unnecessary
	short y;//but maybe they could decrease computational complexity
	char wasvisited;//might be helpful for searching underwatered areas	
	unsigned short waterlvl;//negative if not lawn
	struct _lawn *left;
	struct _lawn *right;
	struct _lawn *up;
	struct _lawn *down;	
	};
typedef struct _lawn lawn;

/*
 * Structure for holding lawn's parameters
 */
struct _parameters
	{
	short xsize;
	short ysize;
	unsigned short time;	//time spent on watering
	short 360radius;	//radius of 360 sprinkler
	double currentmean;
	}
typedef struct _parameters parameters;

/*
 * Calculates arithmetic mean of pixel's waterlvl in Lawn
 *
 * lawn *Lawn - points to (0, 0) lawn pixel
 * 
 * parameters *Param - points to structure that holds 
 * lawn's parameters
 * 
 * Function does not update currentmean field in parameters
 * structure. Returns said mean.
 */
double CalculateMean(lawn *Lawn, parameters *Param);

/*
 * Places sprinkler and waters the lawn
 *
 * lawn *Lawn - points to (0, 0) lawn pixel
 * 
 * short type - sprinkler's type (90/180/270/360)
 * 
 * short x, y - sprinkler's coordinates
 * 
 * double deg - degree between OX and circular's segment "arm" - 
 * the one that is nearer OX when circular segment is placed 
 * with rounded side upwards
 * 
 * sprlist *Sprinklers - list of already placed sprinklers
 */
void PutSprinkler(lawn *Lawn, short type, short x, short y, double deg, sprlist *Sprinklers);

#endif
