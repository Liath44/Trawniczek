#ifndef LAWNHOLDER
#define LAWNHOLDER

#include <stdlib.h>
#include "output.h"

/*
 * Structure for holding lawn's parameters
 */
struct _parameters
	{
	int xsize;
	int ysize;
	int time;	//time spent on watering
	int 360radius;	//radius of 360 sprinkler
	double currentmean;
	}
typedef struct _parameters parameters;

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
void PutSprinkler(char **Lawn, int type, int x, int y, double deg, sprlist *Sprinklers);

#endif
