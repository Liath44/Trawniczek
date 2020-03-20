#ifndef INPUT
#define INPUT

#include <stdlib.h>
#include <stdio.h>
#include "lawnholder.h"

/*
 * Transforms information from lawnfile into Lawn structure. 
 * Updates Param structure with necessary information.
 * 
 * FILE *lawnfile - stores to-be-transformed lawn.
 * Desired format is detailed in documentation enclosed
 * 
 * lawn *Lawn - points to (0, 0) lawn pixel
 * 
 * parameters *Param - points to structure that holds 
 * lawn's parameters. Function updates fields of this structure
 * 
 * Returns 0 on failure
 */
short CreateLawn(FILE *lawnfile, lawn *Lawn, parameters *Param);

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
