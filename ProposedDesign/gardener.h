#ifndef GARDENER
#define GARDENER

#include <stdlib.h>
#include "lawnholder.h"
#include "output.h"

/*
 * Places sprinklers on lawn in order to water it
 * 
 * lawn *Lawn - points to (0, 0) lawn pixel
 * 
 * parameters *Param - points to structure that holds 
 * lawn's parameters
 * 
 * sprlist *Sprinklers - list of to-be-placed placed sprinklers
 * 
 * Returns 0 on failure
 */
short DoTheJob(lawn *Lawn, parameters *Param, sprlist *Sprinklers);

#endif
