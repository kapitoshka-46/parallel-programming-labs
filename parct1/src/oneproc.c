#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SEED 1000 

long long calculate(const int* x, const int* y, int length) {
    long long s = 0;
    for (int i = 0; i < length; i++) {
        for (int j = 0; j < length; j++) {
            s += x[i] * y[j];       
        }
    }
    return s;
}
// comment 
int* create_vector(int length) {
    int* vector = (int*) malloc(sizeof(int) * length);
    if (!vector) return NULL;
    for (int i = 0; i < length; i++) { vector[i] = rand() % 10; }
    return vector;
}

void delete_vector(int** v) {
    if (*v) free(*v);
    *v = NULL;
}

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);
    srand(SEED);
    if (argc != 2) {
        puts("Specify the vector length");
        puts("Usage:");
        printf("    %s [vector length]\n", argv[0]);
        MPI_Finalize();
        return 0;
    }

    const int length = atoi(argv[1]);
    
    int *x = create_vector(length);
    int *y = create_vector(length);
    double timer = MPI_Wtime();

    long long result = calculate(x, y, length);
    
    timer = MPI_Wtime() - timer;
    printf("%lld %.2lf\n", result, timer);
    
    delete_vector(&x); delete_vector(&y);
    
    MPI_Finalize();
    return 0;
}
