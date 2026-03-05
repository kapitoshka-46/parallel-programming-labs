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
