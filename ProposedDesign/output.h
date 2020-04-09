#ifndef OUTPUT
#define OUTPUT

#include <stdlib.h>
#include <stdio.h>
#include "lawnholder.h"
#include "picasso8.h"

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
 * short type - sprinkler's type (90/180/270/360)
 *
 * short x, y - sprinkler's coordinates
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
 * Creates graphical representation of Lawn
 *
 * FILE *bitmap - output file
 *
 * char **Lawn - matrix that stores lawn's pixels
 *
 * parameters *Param - points to structure that holds
 * lawn's parameters
 *
 * Returns 0 on failure
 */
int CreateBitmap(FILE *bitmap, char **Lawn, parameters *Param);

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
