#include <string.h> // for memset
#include <stdio.h>
#include "utility.h"
#include <stdbool.h>

/* Check if point x is close to y in terms of Euclidean distance */
bool is_close(double* x, double* y, double eps, size_t N) {
    // We do not need to take sqrt, because
    // sqrt(|x - y|) < eps) => |x-y| < eps*eps
    double dist = 0;
    for (int i = 0; i < N; i++) {
        dist += (x[i] - y[i]) * (x[i] - y[i]);
    }
    // do not need abs(dist) because dist is a sum of squares
    return dist < eps*eps;
}


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