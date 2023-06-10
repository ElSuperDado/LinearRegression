/**********************************************************
* file: test.c
* authors: Maxence Montavon & Damian Boquete Costa
* description: comparing analytic and numeric solution
***********************************************************/

#include "lib/optimization.h"

// Analytic solution : a = (B - N*(AC-BD / A²-ND)) / A
double compute_a(coordinates* cloud, int nb_dots) {
    double all_x[nb_dots];
    double all_y[nb_dots];
    double all_x_times_y[nb_dots];
    double all_x2[nb_dots];

    for (int i=0; i<nb_dots; i++) {
        all_x[i] = cloud[i].x;
        all_y[i] = cloud[i].y;
        all_x_times_y[i] = all_x[i] * all_y[i];
        all_x2[i] = cloud[i].x * cloud[i].x;
    }

    double A = sum_of(nb_dots-1, 0, all_x);
    double B = sum_of(nb_dots-1, 0, all_y);
    double C = sum_of(nb_dots-1, 0, all_x_times_y);
    double D = sum_of(nb_dots-1, 0, all_x2);

    return (B - nb_dots*((A*C - B*D) / (A*A - nb_dots*D))) / A;
}

// Analytic solution : b = (AC-BD)/(A²-ND)
double compute_b(coordinates* cloud, int nb_dots) {
    double all_x[nb_dots];
    double all_y[nb_dots];
    double all_x_times_y[nb_dots];
    double all_x2[nb_dots];

    for (int i=0; i<nb_dots; i++) {
        all_x[i] = cloud[i].x;
        all_y[i] = cloud[i].y;
        all_x_times_y[i] = all_x[i] * all_y[i];
        all_x2[i] = cloud[i].x * cloud[i].x;
    }

    double A = sum_of(nb_dots, 0, all_x);
    double B = sum_of(nb_dots, 0, all_y);
    double C = sum_of(nb_dots, 0, all_x_times_y);
    double D = sum_of(nb_dots, 0, all_x2);

    return ((A*C - B*D) / (A*A - nb_dots*D));
}

int main() {
    init_random_seed();
    double slope = 2.0 * random_0_to_1_generator();
    double intercept = 2.0 * random_0_to_1_generator();
    double eps = 10E-8; // must be greater than 0
    double lambda = 10E-4; // must be between [0, 1[
    coordinates* cloud = random_cloud_generator(slope, intercept, NB_DOTS);

    double a = compute_a(cloud, NB_DOTS);
    double b = compute_b(cloud, NB_DOTS);

    coordinates random_start = {.x = 0, .y = 0};
    coordinates* numeric_gradient = compute_gradient(cloud, NB_DOTS, random_start, eps, lambda);
    coordinates* analytic_gradient = coor_create(a, b);
    
    printf("\nAnalytic and numeric solutions comparison :\n");
    printf("-------------------------------------------\n");
    printf("Analytique solution slope : %lf\n", analytic_gradient->x);
    printf("Numeric solution slope : %lf\n", numeric_gradient->x);
    printf("Difference between absolute values of slopes = %lf\n\n", fabs(analytic_gradient->x - numeric_gradient->x));
    printf("Analytique solution intercept : %lf\n", analytic_gradient->y);
    printf("Numeric solution intercept : %lf\n", numeric_gradient->y);
    printf("Difference between absolute values of intercept = %lf\n\n", fabs(analytic_gradient->y - numeric_gradient->y));

    export_to_file("data1.txt", cloud, NB_DOTS, *numeric_gradient);
    export_to_file("data2.txt", cloud, NB_DOTS, *analytic_gradient);
    
    free(numeric_gradient);
    free(analytic_gradient);
    free(cloud);
    system("python3 plot.py data1.txt");
    system("python3 plot.py data2.txt");

    return EXIT_SUCCESS;
}