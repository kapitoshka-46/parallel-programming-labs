#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define LOG(format, ...) printf("%d: " format "\n", rank, ##__VA_ARGS__)
void LOG_VECTOR(int rank, const char* name, int* x, int length);

const int TAG_PART_OF_FIRST = 12211;
const int TAG_FULL_SECOND = 12345; 
const int TAG_RESULT = 99999;


long long calculate(const int* x, const int* y, int x_length, int y_length) {
    long long s = 0;
    for (int i = 0; i < x_length; i++) {
        for (int j = 0; j < y_length; j++) {
            s += x[i] * y[j];       
        }
    }
    return s;
}

// calculate length of 1st vector that process with this rank will handle
int part_length(int length, int rank, int size) {
    return (rank == size - 1) ? length / size + length % size : length / size;
}

int* create_vector(int legnth);
void delete_vector(int** v);
void fill_vector(int *vector, int length);



int main(int argc, char **argv) {
    srand(10000);
    MPI_Init(&argc, &argv);

    if (argc != 2) {
        puts("Specify the vector length");
        puts("Usage:");
        printf("    %s [vector length]\n", argv[0]);
        MPI_Finalize();
        return 0;
    }

    int rank;
    int size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);


    const int length = atoi(argv[1]);
   
    // init vectors
    int*global_x = NULL;    
    int *global_y = malloc(sizeof(int) *length);
    if (!global_y) MPI_Abort(MPI_COMM_WORLD, 2);
    
    if (rank == 0) { 
        global_x = malloc(sizeof(int) * length);
        if (!global_x) MPI_Abort(MPI_COMM_WORLD, 1);

        fill_vector(global_x, length);
        fill_vector(global_y, length);
        
        if (size == 1) {
            long long sum = 0;
            sum = calculate(global_x, global_y, length, length);
            printf("==%lld", sum);
            delete_vector(&global_x); delete_vector(&global_y);
            MPI_Finalize(); 
            return 0;
        }
    }

    
    
    // ------------------- size > 1 --------------------
    const int local_x_length = length / size;   // we will calculate remain in rank 0

    int* local_x = malloc(sizeof(int) * local_x_length);
    if (local_x == NULL) MPI_Abort(MPI_COMM_WORLD, 1);

    MPI_Bcast(global_y, length, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter(global_x, local_x_length, MPI_INT, local_x, local_x_length, MPI_INT, 0, MPI_COMM_WORLD);

    long long local_sum = calculate(local_x, global_y, local_x_length, length);
    long long result_sum = 0;
    if (rank == 0) {
        int already_calculated = size * local_x_length;
        local_sum += calculate(global_x + already_calculated, global_y, length - already_calculated, length);
    }
          
    MPI_Reduce(&local_sum, &result_sum, 1, MPI_LONG_LONG, MPI_SUM, 0, MPI_COMM_WORLD);
    
    if (rank == 0) {
        printf("%lld\n", result_sum);
    }

    delete_vector(&local_x);
    delete_vector(&global_y);
    delete_vector(&global_x);
    MPI_Finalize();
    return 0;
}



// ============== Vector functions ===============

void fill_vector(int *vector, int length) {
    for (int i = 0; i < length; i++) { vector[i] = rand() % 10; }
}

int* create_vector(int length) {
    int* vector = malloc(sizeof(int) * length);
    if (!vector) return NULL;
    for (int i = 0; i < length; i++) { vector[i] = rand() % 10; }
    return vector;
}

void delete_vector(int** v) {
    if (*v) free(*v);
    *v = NULL;
}

void LOG_VECTOR(int rank, const char* name, int* x, int length) {
    printf("%d: %s [", rank, name);
        for (int i = 0; i < length; i++) {
            printf("%d ", x[i]);
        }
        printf("]\n");
}
