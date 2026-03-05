#pragma once
 
#include <stddef.h>  // for size_t
#include <stdio.h>
#include <stdlib.h> // for FILE

void read_matrix(FILE* f, double* buf, size_t N) {
    if (!f) perror("Cannot open a file");

    for (int i = 0; i < N * N; i++) {
        int r = fscanf(f, "%lf", &buf[i]);
    }    
}

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
