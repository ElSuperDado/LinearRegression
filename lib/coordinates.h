/**********************************************************
* file: coordinates.h
* authors: Maxence Montavon & Damian Boquete Costa
* description: coordinates library
***********************************************************/

#ifndef _COORDINATES_H_
#define _COORDINATES_H_

#include <stdlib.h>
#include <math.h>

typedef struct coordinates_t {
    double x, y;
}coordinates;

// Returns the norm of a vector
double coor_norm(coordinates c);

// Multiply (c) with (s)
void coor_mult_s(coordinates* c, double s);

// Malloc and returns a coordinate with (x) and (y) values
coordinates* coor_create(double x, double y);

// Substract (c1) by (c2)
void coor_sub(coordinates* c1, coordinates c2);

#endif