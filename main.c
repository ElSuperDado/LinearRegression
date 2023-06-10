/**********************************************************
* file: main.c
* authors: Maxence Montavon & Damian Boquete Costa
***********************************************************/

#include "lib/optimization.h"
#include <unistd.h>

int main() {
    init_random_seed();
    double slope = 2.0 * random_0_to_1_generator();
    double intercept = 2.0 * random_0_to_1_generator();
    double eps = 10E-8; // must be greater than 0
    double lambda = 10E-4; // must be between [0, 1[

    coordinates* cloud = random_cloud_generator(slope, intercept, NB_DOTS);
    coordinates random_start = {.x = 0, .y = 0};
    coordinates* gradient = compute_gradient(cloud, NB_DOTS, random_start, eps, lambda);
    // printf("      GRD-x : %lf    GRD-y : %lf\n", gradient->x, gradient->y);
    double quadra_error = quadratic_error(cloud, NB_DOTS, gradient->x, gradient->y);
    printf("Quadratic error -------> %lf\n", quadra_error);
    
    export_to_file("data.txt", cloud, NB_DOTS, *gradient);
    cross_validation(cloud, NB_DOTS, eps, lambda);
    system("python3 plot.py data.txt data1.txt data2.txt data3.txt");

    free(cloud);
    free(gradient);

    return EXIT_SUCCESS;
}