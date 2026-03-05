#pragma once
 
#include <stddef.h>  // for size_t
#include <stdio.h>
#include <stdlib.h> // for FILE
#include <string.h>

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
    fprintf(f, "\n[matrix %d] %s:\n", cnt, name);
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
    fprintf(f, "\n[vector %d] %s:\n", cnt, name);
    for(int i = 0; i < N; i++) {
        fprintf(f, "%.2lf ", m[i]);
    }
    fprintf(f, "\n");
    if (!to_stderr) fclose(f); 
    cnt++;
}

/* 
    Mults m * v, m: [rows x cols],  v: [cols x 1] 
    m: [rows x cols], double[rows*cols]
    v: [cols x 1], double[cols]
    buf: [rows x 1], double[rows] - buf for result vector
*/
void m_mult_vector(double* m, double* v, double* buf, size_t rows, size_t cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            buf[i] += m[i*rows + j] * v[j];
        }
    }
}

// Substruct m2 from m1
void m_sum(double* buf, const double* m1, double scalar_m1,
                                    const double* m2, double scalar_m2, size_t N) {
    for (int i = 0; i < N; i++) {
        buf[i] = m1[i]*scalar_m1 + m2[i]*scalar_m2;
    }
}

void m_mult_scalar(double scalar, double* m, size_t N) {
    for (int i = 0; i < N; i++) {
        m[i] *= scalar;
    }
}

double m_dot(const double* m1, const double* m2, size_t N) {
    int res = 0;
    for (int i = 0; i < N; i++) {
        res += m1[i] * m2[i];
    }
    return res;
}

void m_copy(double* dst, const double* src, size_t N) {
    memcpy(dst, src, N * sizeof(double));
}