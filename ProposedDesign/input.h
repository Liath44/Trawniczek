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
 * char ***lawn - points to the matrix that is the representation
 * of the lawn
 *
 * Returns error code on failure
 *
 * Returns 0 on success.
 */
int CreateLawn(FILE *lawnfile, parameters *Param, char ***lawn);

/*
 * Initializes Param structure with necessary information
 *
 * FILE *lawnfile - stores information on subject lawn.
 * Desired format is detailed in documentation enclosed
 *
 * parameters *Param - points to structure that holds
 * lawn's parameters. Function initializes fields of this structure
 *
 */
void InitializeParameters(FILE *lawnfile, parameters *Param, int argc, char ** argv);

/*
 * Fills the desired square on matrix with corresponding values
 *
 * char **lawn - pointer to the matrix that will be filled
 *
 * int x, int y - coordinates of the top left corner of the
 * square
 *
 * char c - value that will be used to fill the square
 *
 * int pixel - the side length on the square. Taken from Param
 */
void fillSquare(char **lawn, int x, int y, char c, int pixel);

/*
 * Doubles the size of the matrix
 *
 * parameters *Param - points to structure that holds
 * lawn's parameters. Function updates fields of this structure
 *
 * char ***lawn - points to the matrix that is the representation
 * of the lawn
 *
 */
int BiggerLawn(parameters *Param, char ***lawn);

/*
 * Reduces the size of the matrix so there are not unused spaces
 *
 * parameters *Param - points to structure that holds
 * lawn's parameters. Function updates fields of this structure
 *
 * char ***lawn - points to the matrix that is the representation
 * of the lawn
 *
 * int x, int y - desired size of the matrix, in input file
 * characters. To be multiplied by pixelSize
 *
 */
int ReduceLawn(parameters *Param, char ***lawn, int x, int y);

#endif
