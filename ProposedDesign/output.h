#ifndef OUTPUT
#define OUTPUT

#include <stdlib.h>
#include <stdio.h>
#include "lawnholder.h"
#include "picasso8.h"

/*
 * List of sprinklers
 */
struct _sprlist
	{
	short type;
	short x;
	short y;
	struct _sprlist *next;
	}
typedef struct _sprlist sprlist;

/*
 * Puts subject sprinkler on list
 * 
 * sprlist *Sprinklers - list of already placed sprinklers
 * 
 * short type - sprinkler's type (90/180/270/360)
 * 
 * short x, y - sprinkler's coordinates
 * 
 * Returns 0 on failure
 */
short PutOnList(sprlist *Sprinklers, short type, short x, short y);

/*
 * Creates graphical representation of Lawn
 * 
 * FILE *bitmap - output file
 * 
 * lawn *Lawn - points to (0, 0) lawn pixel
 * 
 * parameters *Param - points to structure that holds 
 * lawn's parameters
 * 
 * Returns 0 on failure
 */
short CreateBitmap(FILE *bitmap, lawn *Lawn, parameters *Param);

/*
 * Creates file that stores information on sprinklers'
 * total number and placements
 * 
 * FILE *output - output file
 * 
 * sprlist *Sprinklers - list of placed sprinklers
 */
void CreateOutputFile(FILE *output, sprlist *Sprinklers)

#endif
