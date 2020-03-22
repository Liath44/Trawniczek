#ifndef GARDENER
#define GARDENER

#include <stdlib.h>
#include "lawnholder.h"
#include "output.h"

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
short DoTheJob(char **Lawn, parameters *Param, sprlist *Sprinklers);

#endif
