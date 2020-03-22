#ifndef INPUT
#define INPUT

#include <stdlib.h>
#include <stdio.h>
#include "lawnholder.h"

/*
 * Transforms information from lawnfile into matrix
 * (0 - non-lawn, 1...n - water level - 1). 
 * Updates Param structure with necessary information.
 * 
 * FILE *lawnfile - stores to-be-transformed lawn.
 * Desired format is detailed in documentation enclosed
 * 
 * parameters *Param - points to structure that holds 
 * lawn's parameters. Function updates fields of this structure
 * 
 * Returns NULL on failure
 * 
 * Returns pointer to created matrix on success.
 */
char **CreateLawn(FILE *lawnfile, parameters *Param);

/*
 * Initializes Param structure with necessary information
 * 
 * FILE *lawnfile - stores information on subject lawn.
 * Desired format is detailed in documentation enclosed
 * 
 * parameters *Param - points to structure that holds 
 * lawn's parameters. Function initializes fields of this structure
 */
void InitializeParameters(FILE *lawnfile, parameters *Param);

#endif
