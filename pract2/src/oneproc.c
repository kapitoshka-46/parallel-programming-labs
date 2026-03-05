#include <stdio.h>
#include "utility.h"
#include <stdbool.h>

void log_matrix(const char* name, double* m, size_t N, bool to_stderr) {
    
    FILE* f = to_stderr ? stderr : fopen("log.txt", "w");
    fprintf(f, "%s:\n", name);
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
            fprintf(f, "%.2lf ", m[i * N + j]);
        }
        fprintf(f, "\n");
    }
    if (!to_stderr) fclose(f); 
}

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