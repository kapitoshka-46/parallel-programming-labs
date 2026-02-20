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

int* create_vector(int legnth);
void delete_vector(int** v);

// calculate length of 1st vector that process with this rank will handle
int part_length(int length, int rank, int size) {
    return (rank == size - 1) ? length / size + length % size : length / size;
}

void process_rank_zero(int length, int x_length, int size) {
    int rank = 0;
    
    // init vectors
    int* x = create_vector(length);
    int* y = create_vector(length);
    if (!x || !y) {
        fputs("Bad malloc. May be not enough memory?", stderr);
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    // --------------- send vectors ---------------------//
    for (int i = 1; i < size; i++) {
        MPI_Send(y, length, MPI_INT, i, TAG_FULL_SECOND, MPI_COMM_WORLD);
        
        int length_send = part_length(length, i, size);
        MPI_Send(x + i*x_length, length_send, MPI_INT, i, TAG_PART_OF_FIRST, MPI_COMM_WORLD);
    }       
    LOG("sended");

    // -------------- calculate the part ----------------//
    long long sum = calculate(x, y, x_length, length);

    // ---------------- merge (recieve) results -------------------//
    for (int i = 1; i < size; i++) {
        int recieve;
        MPI_Recv(&recieve, 1, MPI_LONG_LONG, i, TAG_RESULT, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        sum += recieve;
    }
    printf("%lld\n", sum); 
    delete_vector(&x);
    delete_vector(&y);
}


void process_rank_non_zero(int length, int x_length, int rank, int size) {
    // --------------- recieve vectors -------------------// 
        // ---------------- 2nd vector -------------------
        int* y = malloc(sizeof(int) * length);    
        if (!y) MPI_Abort(MPI_COMM_WORLD, 2);
        MPI_Recv(y, length, MPI_INT, 0, TAG_FULL_SECOND, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        // ---------------- first vector -----------------
        int* x = malloc(sizeof(int) * x_length);
        MPI_Recv(x, x_length, MPI_INT, 0, TAG_PART_OF_FIRST, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        LOG("received");
        // ----------------- calculate -------------------- //
        long long sum = calculate(x, y, x_length, length);
        
        // --------------- merge to rank0 ----------------- //
        MPI_Send(&sum, 1, MPI_LONG_LONG, 0, TAG_RESULT, MPI_COMM_WORLD);
}


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
    const int x_length = part_length(length, rank, size);

    if (rank == 0) {
        process_rank_zero(length, x_length, size);        
    }
    else {  // can't enter this block if size == 1 becuse (rank < size) => (rank == 0)
        process_rank_non_zero(length, x_length, rank, size);
    }
    MPI_Finalize();
    return 0;
}



// ============== Vector functions ===============

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