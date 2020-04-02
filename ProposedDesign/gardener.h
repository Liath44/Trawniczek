#ifndef GARDENER
#define GARDENER

#define JUMP 100

#include <stdlib.h>
#include "lawnholder.h"
#include "output.h"

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
int DoTheJob(char **Lawn, parameters *Param, sprlist *Sprinklers);

#endif
