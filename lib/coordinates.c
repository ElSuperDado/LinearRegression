/**********************************************************
* file: coordinates.c
* authors: Maxence Montavon & Damian Boquete Costa
***********************************************************/

#include "coordinates.h"

coordinates* coor_create(double x, double y) {
    coordinates* vec = malloc(1 * sizeof(coordinates));
    vec->x = x;
    vec->y = y;
    return vec;
}

double coor_norm(coordinates c) {
    return sqrtf(c.x*c.x + c.y*c.y);
}

void coor_sub(coordinates* c1, coordinates c2) {
    c1->x -= c2.x;
    c1->y -= c2.y;
}

void coor_mult_s(coordinates* c, double s) {
    c->x *= s;
    c->y *= s;
}