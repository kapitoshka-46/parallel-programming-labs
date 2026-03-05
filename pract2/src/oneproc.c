#include <stdlib.h>
#include <string.h> // for memset
#include <stdio.h>
#include "utility.h"
#include <stdbool.h>

bool done(double* r, double* b, double eps, size_t N) {
    static double norm_b = 0;
    /* because we use the same b for every check,
        we can calculate it once and then use cached result */
    static double* b_prev = NULL;

    double norm_r = 0;
    if (b_prev != b) norm_b = 0;

    for (int i = 0; i < N; i++) {
        norm_r += r[i]*r[i];
        if (b_prev != b) norm_b += b[i]*b[i];
    }

    b_prev = b;
    return norm_r / norm_b < eps*eps;
}


/* 
   Solves the SLE (System of Linear Equations)
   ax = b. a: NxN, x: Nх1, b: Nx1   (but in memory they are linear arrays) 
   x is a buffer for writing a solution

   This function uses Conjugate gradient method 
*/
void solve(double* a, double* x, double* b, size_t N) {
    memset(x, 0, N);
    
    // r = b - Ax
    double* r = (double*) malloc(sizeof(double) * N);
    double* z = (double*) malloc(sizeof(double) * N);
    double* Az = (double*) malloc(sizeof(double) * N);
    double* Ax = (double*) malloc(sizeof(double) * N);

    double* x_next = (double*) malloc(sizeof(double) * N);
    double* r_next = (double*) malloc(sizeof(double) * N);
    double* z_next = (double*) malloc(sizeof(double) * N);
    double* Az_next = (double*) malloc(sizeof(double) * N);



    if (!r || !z || !Az) {
        perror("solve: cannot allocate data");
        exit(2);
    }


    // Az_next = ax
    m_mult_vector(a, x, Ax, N, N);
    
    // r = b - ax
    m_sum(r, b, 1, Ax, -1, N);
    free(Ax);
    // z = r
    m_copy(z, r, N);


    double betta_next;
    double alfa_next;
    while(!done(r, b, 10e-5, N)) {
        // Az = a * z
        m_mult_vector(a, z, Az, N, N);

        double r_sqr = m_dot(r,  r, N);
        alfa_next = r_sqr / m_dot(Az, z, N);
        betta_next = m_dot(r_next, r_next, N) / r_sqr;

        m_sum(x_next, x, 1, z, alfa_next, N);
        m_sum(r_next, r, 1, Az, -alfa_next, N);
        m_sum(z_next, r_next, 1, z, betta_next, N);

        m_copy(x, x_next, N);
        m_copy(r, r_next, N);
        m_copy(z, z_next, N);
    } 

    log_vector("result", x, N, true);
}


int main() {
    
    size_t N;
    
    FILE* f = fopen("matrix.txt", "r");
    
    if (f == NULL) {
        perror("main: Cannot open file \"matrix.txt\"");
        exit(2);
    }
    fscanf(f, "%lu", &N);
    
    // finding x: ax = b
    double* a = (double*) malloc(sizeof(double) * N * N);
    double* x = (double*) malloc(sizeof(double) * N);
    double* b = (double*) malloc(sizeof(double) * N);
    

    if (!a || !b || !x) {
        perror("Cannot allocate data");
        exit(1);
    }

    read_matrix(f, a, N);
    read_vector(f, b, N);

    solve(a, x, b, N);
    

    
    log_matrix("a", a, N, true);
    log_vector("b", b, N, true);
    // free resources
    fclose(f);
    free(a);
}