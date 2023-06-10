/**********************************************************
* file: optimization.h
* authors: Maxence Montavon & Damian Boquete Costa
* description: optimization library
***********************************************************/

#ifndef _OPTIMIZATION_H_
#define _OPTIMIZATION_H_
#define _XOPEN_SOURCE

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "coordinates.h"

#define NB_DOTS 200

void init_random_seed();

// Generate a random number between 0 and 1
double random_0_to_1_generator();

// Generate a random number between min and max
double random_generator(int min, int max);

// Check if a value is present in an array
bool is_present(int n, int* cloud, int size);

// Returns the sum of all values in the array expression
double sum_of(int n, int i, double* expression);

// Free all the groups in (groups)
void free_groups(coordinates** groups, int nb_groups);

// Returns the derivative related to a
double derivative_a(double x, double y, coordinates* cloud, int nb_dots);

// Returns the derivative related to b
double derivative_b(double x, double y, coordinates* cloud, int nb_dots);

// Generate a number (nb_groups) of groups and randomly gives them cloud values
coordinates** generate_groups(coordinates* cloud, int nb_dots, int nb_groups);

// Generate a list of coordinates with random values
coordinates* random_cloud_generator(double slope, double intercept, int size);

// Compute a cross validation of a cloud of dots
void cross_validation(coordinates* cloud, int nb_dots, double eps, double lambda);

// Calculate the quadratic error
double quadratic_error(coordinates* cloud, int nb_dots, double slope, double intercept);

// Export the list of coordinates in a .txt file
void export_to_file(char* filename, coordinates* cloud, int size, coordinates gradient);

// Fuse two groups together
coordinates* groups_fusion(coordinates* group1, coordinates* group2, int nb_distributed_dots);

// Compute gradient descent
coordinates* compute_gradient(coordinates* cloud, int nb_dots, coordinates random_start, double eps, double lambda);

#endif