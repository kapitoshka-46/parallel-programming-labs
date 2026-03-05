#include <stdio.h>
#include "utility.h"
#include <stdbool.h>


int main() {
    size_t N;
    
    FILE* f = fopen("matrix.txt", "r");
    fscanf(f, "%lu", &N);
    
    double* m = (double*) malloc(sizeof(double) * N * N);
    double* b = (double*) malloc(sizeof(double) * N);
    if (!m || !b) {
        perror("Cannot allocate data");
        if (m) free(m);
        if (b) free(b);
        exit(1);
    }

    read_matrix(f, m, N);
    read_vector(f, b, N);
    
    log_matrix("a", m, N, true);
    log_vector("b", b, N, true);
    // free resources
    fclose(f);
    free(m);
}