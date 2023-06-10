/**********************************************************
* file: optimization.c
* authors: Maxence Montavon & Damian Boquete Costa
***********************************************************/

#include "optimization.h"

void init_random_seed() {
    srand48(time(NULL));
}

double random_0_to_1_generator() {
    return drand48();
}

double random_generator(int min, int max) {
    int r = rand() % max;
    if (r < min) r += min;
    return (double)r;
}

coordinates* random_cloud_generator(double slope, double intercept, int size) {
    coordinates* list = malloc(size * sizeof(coordinates));

    while(size-- > 0) {
        double x = random_0_to_1_generator();
        double r = random_0_to_1_generator();
        double y = slope * x + intercept + r;
        coordinates new_c = {.x=x, .y=y};        
        list[size] = new_c;
    }
    return list;
}

void export_to_file(char* filename, coordinates* cloud, int size, coordinates gradient) {
    FILE* f = fopen(filename, "w");
    char x[20];
    char y[20];
    char comma[2] = ",\0";

    for (int i = 0; i < size; i++) {
        sprintf(x, "%lf", cloud[i].x);
        sprintf(y, "%lf", cloud[i].y);
        fprintf(f, "%s", x);
        fprintf(f, "%s", comma);
        fprintf(f, "%s", y);
        fprintf(f, "%s", comma);
    }

    sprintf(x, "%lf", gradient.x);
    sprintf(y, "%lf", gradient.y);
    fprintf(f, "%s", x);
    fprintf(f, "%s", comma);
    fprintf(f, "%s", y);
    fprintf(f, "%s", comma);

    fclose(f);
}

double sum_of(int n, int i, double* expression) {
    double result = 0;
    for (; i< n; i++) {
        result += expression[i];
    }
    return result;
}

double derivative_a(double x, double y, coordinates* cloud, int nb_dots) {
    double all_x2[nb_dots];
    double all_x[nb_dots];
    double all_x_times_y[nb_dots];

    for (int i=0; i<nb_dots; i++) {
        all_x[i] = cloud[i].x;
        all_x_times_y[i] = cloud[i].x * cloud[i].y;
        all_x2[i] = cloud[i].x * cloud[i].x;
    }
    return (x * sum_of(nb_dots, 0, all_x2)) + (y * sum_of(nb_dots, 0, all_x)) - sum_of(nb_dots, 0, all_x_times_y);
}

double derivative_b(double x, double y, coordinates* cloud, int nb_dots) {
    double all_x[nb_dots];
    double all_y[nb_dots];

    for (int i=0; i<nb_dots; i++) {
        all_x[i] = cloud[i].x;
        all_y[i] = cloud[i].y;
    }
    return (x * sum_of(nb_dots, 0, all_x)) + (nb_dots*y) - sum_of(nb_dots, 0, all_y);
}

coordinates* compute_gradient(coordinates* cloud, int nb_dots, coordinates random_start, double eps, double lambda) {
    coordinates current = random_start;
    coordinates* next = coor_create(.0f, .0f);
    double verif_epsilon;
    
    do {
        double a = derivative_a(current.x, current.y, cloud, nb_dots);
        double b = derivative_b(current.x, current.y, cloud, nb_dots);
        coordinates gradient = { .x = a, .y = b};
        coor_mult_s(&gradient, lambda);
        coor_sub(next, gradient);
        verif_epsilon = fabs(coor_norm(*next) - coor_norm(current));
        current = *next;
    } while (verif_epsilon > eps);
    return next;
}

bool is_present(int n, int* cloud, int size) {
    for (int i = 0; i < size; i++)
        if (n == cloud[i]) return true;

    return false;
}

coordinates** generate_groups(coordinates* cloud, int nb_dots, int nb_groups) {
    int nb_distributed_dots = nb_dots / nb_groups;
    coordinates** cloud_group = malloc(nb_groups * sizeof(coordinates*));
    int distributed_indexes[nb_dots];
    int idx = 0;

    for (int i = 0; i < nb_groups; i++) {
        cloud_group[i] = malloc(nb_distributed_dots * sizeof(coordinates));

        for (int j = 0; j < nb_distributed_dots; j++) {
            int choice = random_generator(0, nb_dots);
           
            if (!is_present(choice, distributed_indexes, nb_dots)) {
                cloud_group[i][j] = cloud[choice];
                distributed_indexes[idx++] = choice;
            }
            else j--;
        }
    }
    return cloud_group;
}

coordinates* groups_fusion(coordinates* group1, coordinates* group2, int nb_distributed_dots) {
    int total_dots = nb_distributed_dots * 2;
    coordinates* fusion = malloc(total_dots * sizeof(coordinates));

    for (int i = 0; i < total_dots; i++) {
        if (i < nb_distributed_dots) fusion[i] = group1[i];
        else fusion[i] = group2[i-nb_distributed_dots];
    }
    return fusion;
}

void free_groups(coordinates** groups, int nb_groups) {
    for (int i = 0; i < nb_groups; i++) {
        free(groups[i]);
    }
    free(groups);
}

void cross_validation(coordinates* cloud, int nb_dots, double eps, double lambda) {
    int nb_groups = 3;
    coordinates** groups = generate_groups(cloud, nb_dots, nb_groups);
    coordinates* g1_2 = groups_fusion(groups[0], groups[1], nb_dots / nb_groups);
    coordinates* g1_3 = groups_fusion(groups[0], groups[2], nb_dots / nb_groups);
    coordinates* g2_3 = groups_fusion(groups[1], groups[2], nb_dots / nb_groups);

    coordinates random_start = { .x = 0.0 , .y = 0.0 };

    coordinates* gradient1_2 = compute_gradient(g1_2, nb_dots / nb_groups, random_start, eps, lambda);
    coordinates* gradient1_3 = compute_gradient(g1_3, nb_dots / nb_groups, random_start, eps, lambda);
    coordinates* gradient2_3 = compute_gradient(g2_3, nb_dots / nb_groups, random_start, eps, lambda);

    export_to_file("data1.txt", groups[2], nb_dots / nb_groups, *gradient1_2);
    export_to_file("data2.txt", groups[1], nb_dots / nb_groups, *gradient1_3);
    export_to_file("data3.txt", groups[0], nb_dots / nb_groups, *gradient2_3);

    double error_g1 = quadratic_error(groups[0], nb_dots / nb_groups, gradient2_3->x, gradient2_3->y);
    printf("Quadratic Error group 1 = %lf\n", error_g1);
    double error_g2 = quadratic_error(groups[1], nb_dots / nb_groups, gradient1_3->x, gradient1_3->y);
    printf("Quadratic Error group 2 = %lf\n", error_g2);
    double error_g3 = quadratic_error(groups[2], nb_dots / nb_groups, gradient1_2->x, gradient1_2->y);
    printf("Quadratic Error group 3 = %lf\n", error_g3);

    free(g1_2);
    free(g1_3);
    free(g2_3);
    free(gradient1_2);
    free(gradient1_3);
    free(gradient2_3);
    free_groups(groups, nb_groups);
}

double quadratic_error(coordinates* cloud, int nb_dots, double slope, double intercept) {
    double res = 0;

    for (int i=0; i<nb_dots; i++) {
        res += ((slope * cloud[i].x) + (intercept - cloud[i].y)) * ((slope * cloud[i].x) + (intercept - cloud[i].y));
    }

    return res;
}