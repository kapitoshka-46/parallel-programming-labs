#pragma once
 
#include <stddef.h>  // for size_t
#include <stdio.h>
#include <stdlib.h> // for FILE

// Reads N*N doubles form file
void read_matrix(FILE* f, double* buf, size_t N) {
    if (!f) fprintf(stderr, "Invalid file");

    for (int i = 0; i < N * N; i++) {
        int r = fscanf(f, "%lf", &buf[i]);
    }    
}

// Reads N doubles from file
void read_vector(FILE* f, double* buf, size_t N) {
    if (!f) fprintf(stderr, "Invalid file");
    for (int i = 0; i < N; i++) {
        int r = fscanf(f, "%lf", &buf[i]);
    }
}

void log_matrix(const char* name, double* m, size_t N, bool to_stderr) {
    static int cnt = 0;
    static const char* mode;
    mode = cnt == 0 ? "w" : "a";

    FILE* f = to_stderr ? stderr : fopen("log.txt", "w");
    fprintf(f, "%d. %s:\n", cnt, name);
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
            fprintf(f, "%.2lf ", m[i * N + j]);
        }
        fprintf(f, "\n");
    }
    if (!to_stderr) fclose(f); 
    
    cnt++;
}

void log_vector(const char* name, double* m, size_t N, bool to_stderr) {
    
    static int cnt = 0;
    static const char* mode;
    mode = cnt == 0 ? "w" : "a";    // first create file and then append

    FILE* f = to_stderr ? stderr : fopen("log_vector.txt", "w");
    fprintf(f, "%d. %s:\n", cnt, name);
    for(int i = 0; i < N; i++) {
        fprintf(f, "%.2lf ", m[i]);
    }
    fprintf(f, "\n");
    if (!to_stderr) fclose(f); 
    cnt++;
}
