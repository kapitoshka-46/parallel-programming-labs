#include <stdio.h>
#include "utility.h"
#include <stdbool.h>


int main() {
    size_t N;
    
    FILE* f = fopen("matrix.txt", "r");
    fscanf(f, "%lu", &N);
    
    double* m = (double*) malloc(sizeof(double) * N * N);
    read_matrix(f, m, N);



    // free resources
    fclose(f);
    free(m);
}