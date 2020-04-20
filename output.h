#ifndef OUTPUT
#define OUTPUT

#include <stdlib.h>
#include <stdio.h>
#include "lawnholder.h"
#include "picasso8.h"

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
void CreateOutputFile(FILE *output, sprlist *Sprinklers);

#endif
